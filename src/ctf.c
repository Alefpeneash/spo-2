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
#include "state.h"
#include "options.h"
#include "fd.h"
#include "copying.h"
#include "ctd.h"
#include "gv.h"

/* called when copying to file (calls copying function) */
int copying_to_file(argv)
char* argv[];
{
	time_t currtime = time(NULL); 
	fd args;	

	if ((args.source = open(argv[1], O_RDONLY)) == -1)
	{
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
	struct stat source_stat;
	fstat(args.source, &source_stat);
	if((args.dest = open(argv[2], O_CREAT | O_WRONLY, source_stat.st_mode)) == -1)
	{
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
	struct stat dest_stat;	
	fstat(args.dest, &dest_stat);
	
	if (currtime > dest_stat.st_mtim.tv_sec)
	{
		exit(EXIT_SUCCESS);
		close(args.source);
		close(args.dest);
	}

	if (opts.verbose == any)
	{
		printf("'%s' -> '%s'\n", argv[1], argv[2]);	
	}

	copying(&args);	

	close(args.source);
	close(args.dest);
	
	return 0;
}

