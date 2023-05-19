#include "open_file.h"

FILE *open_file(char *file_path, char *mode)
{
    FILE *fp = fopen(file_path, mode);
    if (fp == NULL)
    {
        printf("Error opening file %s in mode %s\n", file_path, mode);
        return NULL;
    }
    return fp;
}
