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
	progname = argv[0];
	copying(argc, argv);	

	exit(0);
}


int copying(argc, argv)
int argc;
char* argv[];
{
	int arg_counter;
	char buf[BUFSIZ];
	int source[argc - 2];
	int dest[argc - 2];
	char const SLASH = '/';
	char const END_OF_STRING='\0';
	char* const dir = argv[argc - 1];

	for (int i = 0; i < argc - 2; i++)
	{	

		if ((source[i] = open(argv[i + 1], O_RDONLY)) == -1)
			error("can't open %s", argv[i + 1]);

		char* path = (char*) malloc((char)(strlen(dir) + strlen(argv[i + 1])) + sizeof(char));// len of dir and filenames + 1 charsize for '/'

		strcpy(path, dir);
		if (*(path + (char)strlen(dir) - sizeof(char)) != SLASH)
		{
			*(path + (char)strlen(dir)) = SLASH;
			*(path + (char)strlen(dir) + sizeof(char)) = END_OF_STRING;
		}
		strcat(path, argv[i + 1]);

		if ((dest[i] = open(path, O_CREAT | O_WRONLY)) == -1)
			error("can't open or create %s", path);		

		free(path);

		while((arg_counter = read(source[i], buf, BUFSIZ)) > 0)
			if(write(dest[i], buf, arg_counter) != arg_counter)
				error("write error", (char *) 0);

		close(source[i]);
		close(dest[i]);
	}

}
