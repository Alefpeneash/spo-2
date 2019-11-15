#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

char* progname;

int main(argc, argv)
int argc;
char* argv[];
{
	int f1, f2, n;
	char buf[BUFSIZ];
	progname = argv[0];
	
	if (argc < 3)
		error("Usage: %s from to", progname);
	if ((f1 = open(argv[1], O_RDONLY)) == -1)
		error("can't open %s", argv[1]);
	if ((f2 = open(argv[2], O_CREAT | O_WRONLY)) == -1)
		error("can't open or create %s", argv[2]);

	while((n = read(f1, buf, BUFSIZ)) > 0)
		if(write(f2, buf, n) != n)
			error("write error", (char *) 0);
	
	close(f1);
	close(f2);

	exit(0);
}
