#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include "gv.h"
#include "fd.h"
#include "copying.h"
#include "state.h"
#include "options.h"
#include "errg.h"

#define CURRENT_DIR 	"."
#define PREVIOUS_DIR 	".."
#define SLASH			"/"	


void copy_recursively(source_dir_path, dest_dir_path)
char* source_dir_path;
char* dest_dir_path;
{

    char* source_basename = malloc((strlen(basename(source_dir_path)) + 1) * sizeof(char));
    strcpy(source_basename, basename(source_dir_path)); 

	DIR *source_dir;
	if ((source_dir = opendir(source_dir_path)) == NULL)
	{
		perror(progname);
		exit(EXIT_FAILURE);
	}

	struct stat *source_dir_stat = malloc(sizeof(struct stat)); 
	stat(source_dir_path, source_dir_stat);

    int check = 0;
	
    if (opendir(dest_dir_path) == NULL)
    {
        check = 1;        

        char* full_dest_path = malloc(sizeof(char) * strlen(dest_dir_path));
        strcpy(full_dest_path, dest_dir_path);
        
	    mkdir(full_dest_path, source_dir_stat->st_mode);
	    free(full_dest_path);
    }	
    else{
	char* full_dest_path = malloc(sizeof(char) * (strlen(source_basename) + strlen(dest_dir_path)));
	strcpy(full_dest_path, dest_dir_path);
	strcat(full_dest_path, SLASH);
	strcat(full_dest_path, source_basename);
    
	//check if exist first

	mkdir(full_dest_path, source_dir_stat->st_mode);
	free(full_dest_path);
   // free(source_basename);
	}
    struct dirent *ent = malloc(sizeof(struct dirent));

	while ((ent = readdir(source_dir)) != NULL)
	{
        int* errn = malloc(sizeof(int));
        
		if (strcmp(ent->d_name, CURRENT_DIR) == 0 || strcmp(ent->d_name, PREVIOUS_DIR) == 0)
			continue;

		char* source_file = malloc(sizeof(char) * (strlen(source_dir_path) + strlen(ent->d_name)));
		strcpy(source_file, source_dir_path);
		strcat(source_file, SLASH);
		strcat(source_file, ent->d_name);

		struct stat *source_stat = malloc(sizeof(struct stat)); 
		fd *args = malloc(sizeof(fd));

        if (access(source_file, R_OK) != 0)
        {    
            *errn = SOURCE_PRIVIL;        

            if (access(source_file, F_OK) != 0)
                *errn = SOURCE_EXIST;
        }   

		if ((args->source = open(source_file, O_RDONLY)) == -1)
		{
			perror(error_gen(source_file, *errn));
			//exit(EXIT_FAILURE);
            continue;
		}

		fstat(args->source, source_stat);

		if (S_ISDIR(source_stat->st_mode))
		{
			copy_recursively(source_file, dest_dir_path);
		}
		else
		{
            char* dest_file = malloc(sizeof(char) * ( \
            strlen(dest_dir_path) + \
            strlen(source_dir_path) + \
            strlen(ent->d_name)));

            if (check == 0)
            {
                strcpy(dest_file, dest_dir_path);
                if (*(dest_file + strlen(dest_file) - 1) != '/')
                   strcat(dest_file, SLASH);
                strcat(dest_file, source_basename);
                strcat(dest_file, SLASH);
                strcat(dest_file, ent->d_name);
            }
            else
            {
                strcpy(dest_file, dest_dir_path);
                if (*(dest_file + strlen(dest_file) - 1) != '/')
                   strcat(dest_file, SLASH);

                strcat(dest_file, ent->d_name);
            }
        
            if ((args->dest = open(dest_file, O_CREAT | O_WRONLY, source_stat->st_mode)) == -1)
            {
                perror(progname);
                exit(EXIT_FAILURE);
            }

             
            if (opts.verbose == any)
            {
                printf("'%s' -> '%s'\n", source_file, dest_file);
            }

			/* use pthread to implement multithreading copying */
			pthread_t *tid = malloc(sizeof(pthread_t));
			pthread_create(tid, NULL, copying, (void *)args);
			pthread_join(*tid, NULL);
		
            close(args->source);
            close(args->dest);
            
            free(errn);
            free(tid);
            free(args);
		}

		free(source_file);
		free(source_stat);
	}
    free(source_basename);
	free(source_dir_stat);
	free(ent);
	closedir(source_dir);

}
