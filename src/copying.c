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
#include "ctd.h"
#include "gv.h"

/* function that implements the copy proccess */
void *copying(void *args)
{
	int count;
	char buf[BUFSIZ];

	while ((count = read(((fd*)args)->source, buf, BUFSIZ)) > 0)
		if (write(((fd*)args)->dest, buf, count) != count)
			perror(progname);

	pthread_exit(NULL);
}
