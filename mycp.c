#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

char* progname;

struct fd
{
	int dest;
	int source;
};

void *copying(void *args)
{
	int arg_counter;
	char buf[BUFSIZ];

	while ((arg_counter = read(((struct fd*)args)->source, buf, BUFSIZ)) > 0)
		write(((struct fd*)args)->dest, buf, arg_counter);

	pthread_exit(NULL);
}

int copying_to_dir(argc, argv)
int argc;
char* argv[];
{
	struct fd *args[argc - 1];
	char const SLASH = '/';
	char const END_OF_STRING='\0';
	char* const dir = argv[argc - 1];

	for (int i = 0; i < argc - 2; i++)
	{	

		args[i]->source = open(argv[i + 1], O_RDONLY);

		/* take a source file name memory is allocated with a margin (probably wrong) */
		char* sfilename = malloc((char)(strlen(argv[i + 1])));
		strcpy(sfilename, basename(argv[i + 1]));
	
		/* this structure is needed because it contains the privileges of the source file */
		struct stat *source_stat = malloc(sizeof(struct stat)); 
		fstat(args[i]->source, source_stat); 

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

		args[i]->dest = open(path, O_CREAT | O_WRONLY, source_stat->st_mode);
		
		free(sfilename);
		free(source_stat);
		free(path);

		/* use pthread to implement multithreading copying */
		pthread_t tid;
		pthread_create(&tid, NULL, copying, (void *)args[i]);
		pthread_join(tid, NULL);

		close(args[i]->source);
		close(args[i]->dest);
	}
	return 0;
}

int copying_to_file(argv)
char* argv[];
{
	struct fd args;	

	args.source = open(argv[1], O_RDONLY);
	struct stat source_stat; 
	fstat(args.source, &source_stat);
	args.dest = open(argv[2], O_CREAT | O_WRONLY, source_stat.st_mode);

	copying(&args);	

	close(args.source);
	close(args.dest);
	
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
		copying_to_dir(argc, argv);
		exit(0);
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
			copying_to_file(argv);
			break;
		default:
			copying_to_dir(argc, argv);
	} 

	exit(0);
}
