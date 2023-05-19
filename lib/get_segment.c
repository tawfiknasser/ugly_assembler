#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../config.h"

/**
* @return get specific segment. ignoring spaces.
*/
char *get_segment(char *line_buffer, int segment_number)
{

    char *current_segment = NULL;
    char *end;
    char *walker = line_buffer;
    size_t current_segment_length = 0;
    int i = 0;
    while (i < segment_number)
    {
        free(current_segment); /*free previous allocation*/
        current_segment_length = 0;
        if (*walker == NULL_TERMINATOR)
            exit(1);

        while (*walker != NULL_TERMINATOR && isspace(*walker))
        {
            walker++;
        }

        end = walker;
        while (*end != NULL_TERMINATOR && !isspace(*end))
        {
            current_segment_length++;
            end++;
        }

        current_segment = malloc(current_segment_length + 1);
        if (current_segment == NULL)
        {
            printf("failed to allocate memory for segment");
            exit(1);
        }
        strncpy(current_segment, walker, current_segment_length);
        current_segment[current_segment_length] = NULL_TERMINATOR;
        walker = end;
        i++;
    }
    return current_segment;
}
