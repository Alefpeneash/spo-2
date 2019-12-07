#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <unistd.h> 
#include "ctd.h"
#include "ctf.h"
#include "cr.h"
#include "state.h"
#include "options.h"
#include "gv.h"

char* progname;
struct options opts = {none, none, none, none};

int main(argc, argv)
int argc;
char* argv[];
{

	progname = argv[0];
	
	int opt;
	int flagcounter = 0;
	
	while ((opt = getopt(argc, argv, "vurf")) != -1)
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
			case 'r':
				flagcounter++;
				opts.recursive = any;
				break;
            case 'f':
                flagcounter++;
                opts.force = any;
                break;
			case '?':
				exit(EXIT_FAILURE);		
		}
	}

    if (flagcounter != 0)
        flagcounter = flagcounter - strlen(argv[1]) + 2;

    int argcp = argc - flagcounter ;

	char* argvp[argcp];
	argvp[0] = malloc((char)(strlen(argv[0]) + 1));
	strcpy(argvp[0], argv[0]);	

	for(int i = 1; i < argcp; i++)
	{
		argvp[i] = argv[i + flagcounter];
	}	


	struct stat dest_stat; 
	stat(argvp[argcp - 1], &dest_stat);
    struct stat source_stat;
    stat(argvp[1], &source_stat); 
	
	if (S_ISDIR(dest_stat.st_mode) || S_ISDIR(source_stat.st_mode))
	{
		copying_to_dir(argcp, argvp);
		exit(EXIT_SUCCESS);
	}
	
	switch (argcp)
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
