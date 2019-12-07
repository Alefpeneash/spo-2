#ifndef ERRG_HEADER
#define ERRG_HEADER

enum errs
{
    NO_ERR,
    SOURCE_EXIST,
    SOURCE_PRIVIL,
    DEST_PRIVIL,
    WITHOUT_R,
    SAME_FILE
};

char* error_gen(char* obj, int errn);

#endif
