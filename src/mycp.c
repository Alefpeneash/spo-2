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
#include "ctd.h"
#include "ctf.h"
#include "state.h"
#include "options.h"
#include "fd.h"
#include "copying.h"
#include "gv.h"

char* progname;
struct options opts = {none, none};

int main(argc, argv)
int argc;
char* argv[];
{

	progname = argv[0];
	
	//recdir(arg[1]);	

	//exit(EXIT_SUCCESS);
	
	//char** elems;
	//int* count = malloc(sizeof(int));
	//*count = 1;
	//elems = malloc(sizeof(char*));
	//elems[0] = malloc((strlen(argv[1]) + 1) * sizeof(char));
	//elems[0] = argv[1];

	//recursive_dir_opener(elems, count);

	//exit(EXIT_SUCCESS);
	
	int opt;
	int flagcounter;
	
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
	
	struct stat dest_stat; 
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
