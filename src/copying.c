#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include "fd.h"
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
