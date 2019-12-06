#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> 
#include "state.h"
#include "options.h"
#include "fd.h"
#include "copying.h"
#include "gv.h"
#include "errg.h"

/* called when copying to file (calls copying function) */
int copying_to_file(argv)
char* argv[];
{
	time_t currtime = time(NULL); 
	fd args;	

    int errn = NO_ERR;

    if (access(argv[1], R_OK) != 0)
    {    
        errn = SOURCE_PRIVIL;        

        if (access(argv[1], F_OK) != 0)
            errn = SOURCE_EXIST;
    }   

	if ((args.source = open(argv[1], O_RDONLY)) == -1)
	{
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
    
    if (access(argv[2], R_OK) != 0)
    {
        errn = DEST_PRIVIL;

        if (opts.force == any)
            remove(argv[2]);
    }
	
    struct stat source_stat;
	fstat(args.source, &source_stat);
	if((args.dest = open(argv[2], O_CREAT | O_WRONLY, source_stat.st_mode)) == -1)
	{
		perror(error_gen(argv[1], errn));
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
        if (errn == DEST_PRIVIL)
            printf("removed '%s'\n", argv[2]);
	}

	copying(&args);	

	close(args.source);
	close(args.dest);
	
	return 0;
}

