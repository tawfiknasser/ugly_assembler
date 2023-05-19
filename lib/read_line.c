#include "read_line.h"
#include "../config.h"
#include <stdlib.h>

/**
* @return char pointer to the start of the line or NULL in case of EOF
*/
char *read_line(FILE *fp)
{
    char *line_buffer = (char *)malloc(MAX_SOURCE_LINE_LENGTH * sizeof(char));
    if (line_buffer == NULL)
    {
        printf("Memory allocation failure in read line.\n");
        exit(1);
    }

    if (fgets(line_buffer, MAX_SOURCE_LINE_LENGTH, fp) == NULL)
    {
        free(line_buffer);
        return NULL;
    }

    return line_buffer;
}