#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

struct fd
{
	int dest;
	int source;
};

struct options
{
	int symlink;
	int hardlink;
};

enum state
{
	none,
	any
};

char* progname;
struct options opts = {none, none};


/* function that implements the copy proccess */
void *copying(void *args)
{
	int arg_counter;
	char buf[BUFSIZ];

	while ((arg_counter = read(((struct fd*)args)->source, buf, BUFSIZ)) > 0)
		write(((struct fd*)args)->dest, buf, arg_counter);

	pthread_exit(NULL);
}

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

		if (opts.hardlink == any)
		{
			return 0;	
		}	

		struct fd *args = malloc(sizeof(struct fd));
	
		args->source = open(argv[i + 1], O_RDONLY);

		/* take a source file name memory is allocated with a margin (probably wrong) */
		char* sfilename = malloc((char)(strlen(argv[i + 1])));
		strcpy(sfilename, basename(argv[i + 1]));
	
		/* this structure is needed because it contains the privileges of the source file */
		struct stat *source_stat = malloc(sizeof(struct stat)); 
		fstat(args->source, source_stat); 

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

		args->dest = open(path, O_CREAT | O_WRONLY, source_stat->st_mode);
		
		free(sfilename);
		free(source_stat);
		free(path);

		/* use pthread to implement multithreading copying */
		pthread_t *tid = malloc(sizeof(pthread_t));
		pthread_create(tid, NULL, copying, (void *)args);
		pthread_join(*tid, NULL);
		
		free(tid);

		close(args->dest);
		close(args->dest);
		
		free(args);
	}
	return 0;
}

/* called when copying to file (calls copying function) */
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
	int opt;	
	struct stat dest_stat; 
	stat(argv[argc - 1], &dest_stat);

	/*
	-s, --symbolic-link
        make symbolic links instead of copying
	-u, --update
        copy only when the SOURCE file is newer than the destination file or when the destination file is missing
    -v, --verbose
        explain what is being done
	-l, --link
        hard link files instead of copying
	*/

	while ((opt = getopt(argc, argv, "sl")) != -1)
	{
		switch (opt)
		{
			case 's':
				opts.symlink = any;
				break;
			case 'l':
				opts.hardlink = any;
				break;
			case '?':
				printf("hz\n");			
		}
	}

	if (opts.symlink == any && opts.hardlink == any)
	{
		printf("huinya");
		exit(0);
	}

	//exit(0);

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
