#include <time.h>
#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "state.h"
#include "options.h"
#include "fd.h"
#include "copying.h"
#include "gv.h"

/* called when copying to a directory(calls copying function in different threads) */
int copying_to_dir(argc, argv)
int argc;
char* argv[];
{
	char const SLASH = '/';
	char const END_OF_STRING='\0';
	char* const dir = argv[argc - 1];

	for (int i = 0; i < argc - 2; i++)
	{
		time_t currtime = time(NULL);

		/* take a source file name memory is allocated with a margin (probably wrong) */
		char* sfilename = malloc((char)(strlen(argv[i + 1])));
		strcpy(sfilename, basename(argv[i + 1]));
		
		/* memory allocation is based on: length of dest path + length of source file name + '/' */
		char* path = malloc((char)(strlen(dir) + strlen(argv[i + 1])) + sizeof(char));

		strcpy(path, dir);
		/* here it checks for the presence of '/' at the end of the destination directory path  */
		if (*(path + (char)strlen(dir) - sizeof(char)) != SLASH)
		{
			*(path + (char)strlen(dir)) = SLASH;
			*(path + (char)strlen(dir) + sizeof(char)) = END_OF_STRING;
		}
		/* concatenate the source file name to the end */
		strcat(path, sfilename);
		
		/* this structure is needed because it contains the privileges of the source file */
		struct stat *source_stat = malloc(sizeof(struct stat)); 
		struct stat *dest_stat = malloc(sizeof(struct stat)); 	
		fd *args = malloc(sizeof(fd));
		if ((args->source = open(argv[i + 1], O_RDONLY)) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		fstat(args->source, source_stat); 	
		if ((args->dest = open(path, O_CREAT | O_WRONLY, source_stat->st_mode)) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		fstat(args->dest, dest_stat);


		if (currtime > dest_stat->st_mtim.tv_sec)
		{
			free(sfilename);
			free(path);
			free(source_stat);
			free(dest_stat);
			close(args->source);
			close(args->dest);
			free(args);
			continue;			
		}
			
		if (opts.verbose == any)
		{
			printf("'%s' -> '%s'\n", argv[i + 1], path);
		}
	
		free(sfilename);
		free(path);
		free(source_stat);
		free(dest_stat);
		
		/* use pthread to implement multithreading copying */
		pthread_t *tid = malloc(sizeof(pthread_t));
		pthread_create(tid, NULL, copying, (void *)args);
		pthread_join(*tid, NULL);
		
		close(args->source);
		close(args->dest);
		
		free(tid);
		free(args);
	}
	return 0;
}
