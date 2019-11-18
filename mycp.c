#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

char* progname;

int multiple_copying(argc, argv)
int argc;
char* argv[];
{
	int arg_counter;
	char buf[BUFSIZ];
	int source[argc - 2]; //should be rewiritten if multi-threads aren't used
	int dest[argc - 2];
	char const SLASH = '/';
	char const END_OF_STRING='\0';
	char* const dir = argv[argc - 1];

	for (int i = 0; i < argc - 2; i++)
	{	

		source[i] = open(argv[i + 1], O_RDONLY);

 		readlink(source[i]);//name of file here
	
		struct stat *source_stat = malloc(sizeof(struct stat)); 
		fstat(source[i], source_stat);

		char* path = (char*) malloc((char)(strlen(dir) + strlen(argv[i + 1])) + sizeof(char));// len of dir and filenames + 1 charsize for '/'

		strcpy(path, dir);
		if (*(path + (char)strlen(dir) - sizeof(char)) != SLASH)
		{
			*(path + (char)strlen(dir)) = SLASH;
			*(path + (char)strlen(dir) + sizeof(char)) = END_OF_STRING;
		}
		strcat(path, argv[i + 1]);

		dest[i] = open(path, O_CREAT | O_WRONLY, source_stat->st_mode);//doesn't work

		free(source_stat);
		free(path);

		while ((arg_counter = read(source[i], buf, BUFSIZ)) > 0)
			write(dest[i], buf, arg_counter);

		close(source[i]);
		close(dest[i]);
	}

	return 0;
}

int single_copying(argv)
char* argv[];
{
	char buf[BUFSIZ];

	int check;
	int source;
	int dest;

	struct stat source_stat; 
	fstat(source, &source_stat);


	source = open(argv[1], O_RDONLY);
	dest = open(argv[2], O_CREAT | O_WRONLY, source_stat.st_mode);

	while ((check = read(source, buf, BUFSIZ)) > 0)
		write(dest, buf, check);

	close(source);
	close(dest);
	
	return 0;
}

int main(argc, argv)
int argc;
char* argv[];
{
	progname = argv[0];
	
	struct stat dest_stat; 
	stat(argv[argc - 1], &dest_stat);

	if (S_ISDIR(dest_stat.st_mode))
	{
		multiple_copying(argc, argv);
	}
	
	switch (argc)
	{
		case 1:
			printf("at least 2 arguments");
			break;
		case 2:
			printf("at least 2 arguments");
			break;
		case 3:
			single_copying(argv);
			break;
		default:
			multiple_copying(argc, argv);
	} 

	exit(0);
}
