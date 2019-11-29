#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "gv.h"

void copy_recursively(dir_name)
char* dir_name;
{
	DIR *dir;
	if ((dir = opendir(dir_name)) == NULL)
	{
		perror(progname);
		exit(EXIT_FAILURE);
	}

	struct dirent *ent = malloc(sizeof(struct dirent));

	while ((ent = readdir(dir)) != NULL)
	{
		printf("%s\n", ent->d_name);
	}
	
	free(ent);
	closedir(dir);
}
