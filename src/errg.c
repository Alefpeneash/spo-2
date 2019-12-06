#include <stdlib.h>
#include <string.h>
#include "gv.h"
#include "errg.h"


char* error_gen(obj, errn)
char* obj;
int errn;
{
    char* str = malloc(sizeof(char) * 50);

    switch(errn)
    {
        case SOURCE_EXIST:
            strcpy(str, progname);
            strcat(str, ": cannot stat '");
            strcat(str, obj);
            strcat(str, "'");
            break;

        case SOURCE_PRIVIL:
            strcpy(str, progname); 
            strcat(str, ": cannot open file '");
            strcat(str, obj);
            strcat(str, "' for reading");
            break;

        case DEST_PRIVIL:
            strcpy(str, progname); 
            strcat(str, ": cannot create regular file '");
            strcat(str, obj);
            strcat(str, "'");
            break; 
    }

    return str;
}
