#ifndef ERRG_HEADER
#define ERRG_HEADER

enum errs
{
    NO_ERR,
    SOURCE_EXIST,
    SOURCE_PRIVIL,
    DEST_PRIVIL
};

char* error_gen(char* obj, int errn);

#endif
