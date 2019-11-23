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

struct fd
{
	int dest;
	int source;
};

struct options
{
	int verbose;
	int update;
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
		struct fd *args = malloc(sizeof(struct fd));
		if ((args->source = open(argv[i + 1], O_RDONLY)) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		if ((args->dest = open(path, O_CREAT | O_WRONLY, source_stat->st_mode)) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		fstat(args->source, source_stat); 	
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

/* called when copying to file (calls copying function) */
int copying_to_file(argv)
char* argv[];
{
	time_t currtime = time(NULL); 
	struct fd args;	

	if ((args.source = open(argv[1], O_RDONLY)) == -1)
	{
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
	struct stat source_stat;
	fstat(args.source, &source_stat);
	if((args.dest = open(argv[2i], O_CREAT | O_WRONLY, source_stat.st_mode)) == -1)
	{
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
	struct stat dest_stat;	
	fstat(args.dest, &dest_stat);
	
	if (currtime > dest_stat->st_mtim.tv_sec)
		exit(EXIT_SUCCESS);

	if (opts.verbose == any)
	{
		printf("'%s' -> '%s'\n", argv[1], argv[2]);	
	}

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
	int flagcounter;
	struct stat dest_stat; 

	while ((opt = getopt(argc, argv, "vu")) != -1)
	{
		switch (opt)
		{
			case 'v':
				flagcounter++;
				opts.verbose = any;
				break;
			case 'u':
				flagcounter++;
				opts.update = any;	
				break;
			case '?':
				exit(EXIT_FAILURE);		
		}
	}
	
	int argcp = argc - flagcounter;
	char* argvp[argcp];
	argvp[0] = argv[0];	

	for(int i = 1; i < argcp; i++)
	{
		argvp[i] = argv[i + flagcounter];
	}	
	
	stat(argvp[argcp - 1], &dest_stat);

	if (S_ISDIR(dest_stat.st_mode))
	{
		copying_to_dir(argcp, argvp);
		exit(EXIT_SUCCESS);
	}
	
	switch (argc)

	{
		case 1:
			printf("%s: missing file operand\n", argv[0]);
			exit(EXIT_FAILURE);
		case 2:
			printf("%s: missing destenation file operand after '%s'\n", argv[0], argv[1]);
			exit(EXIT_FAILURE);
		case 3:
			copying_to_file(argvp);
			break;
		default:
			copying_to_dir(argcp, argvp);
	} 

	exit(EXIT_SUCCESS);
}
