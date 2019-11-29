#include <dirent.h>
#include <time.h>
#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "ctf.h"
#include "state.h"
#include "options.h"
#include "fd.h"
#include "mycp.h"
#include "copying.h"
#include "ctd.h"

void recdirop(dir)
char* arg; 
{
	DIR *dir = opendir(*arg);
	struct dirent *ent = malloc(sizeof(struct dirent));

	while ((ent = readdir(dir)) != NULL)
	{
		if ((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0))
			continue;
		
		char* file = malloc((strlen(ent->d_name) + (strlen(*arg) + 2) * sizeof(char)));

		strcpy(elems[*count - 1], *arg);
		strcat(elems[*count - 1], "/");
		strcat(elems[*count - 1], ent->d_name);

		struct stat *file_stat = malloc(sizeof(struct stat));
		stat(elems[*count - 1], file_stat);
		if (S_ISDIR(file_stat->st_mode))
		{
			free(file_stat);
			recursive_dir_opener(elems, count);
		}
		free(file_stat);
	}
	closedir(dir);
}

