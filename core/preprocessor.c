#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preprocessor.h"
#include "../config.h"
#include "../lib/linked_list.h"
#include "../lib/open_file.h"
#include "../lib/registers.h"
#include "../lib/read_line.h"
#include "../lib/get_segment.h"

/**
 * preprocessing the file. expans macros and ignore empty lines.
*/
int preprocessor(FILE *source_file, char *file_name)
{
    char *output_file_name;
    char *line_buffer = NULL;
    char *segment = NULL;
    size_t line_size = 0;

    FILE *preprocessor_output_fp = NULL;

    node *macros_list = NULL;
    node *macro_node = NULL;

    output_file_name = (char *)malloc(sizeof(char) * (strlen(file_name) + strlen(PREPROCESS_EXTENSION) + 1));
    if (output_file_name == NULL)
    {
        printf("Failed to allocate memory for preprocess file name.\n");
        exit(1);
    }
    strcpy(output_file_name, file_name);
    strcat(output_file_name, PREPROCESS_EXTENSION);
    preprocessor_output_fp = open_file(output_file_name, "w");

    while ((line_buffer = read_line(source_file)) != NULL)
    {
        segment = get_segment(line_buffer, 1); /*segments are freed when cleaning the linked list*/
        /* ignore comments and empty lines */
        if (strcmp(segment, COMMENT_CHAR) == 0 || *segment == NULL_TERMINATOR)
            goto end_of_loop;

        if (PSW.IS_MCR)
        {
            /* end of macro*/
            if (strcmp(segment, END_MCR) == 0)
            {
                PSW.IS_MCR = 0;
                goto end_of_loop;
            }
            /*add line to macro table definition*/
            line_size = strlen(line_buffer) + strlen(macros_list->value.string_value) + 2;
            macros_list->value.string_value = realloc(macros_list->value.string_value, line_size);
            if (macros_list->value.string_value == NULL)
            {
                printf("failed to re-allocate memory");
                exit(1);
            }
            strcat(macros_list->value.string_value, line_buffer);
            goto end_of_loop;
        }
        if (strcmp(segment, MCR) == 0)
        { /*start of macro definition*/
            PSW.IS_MCR = 1;
            /* add macro name to macros table */
            macros_list = add_node_to_head(macros_list);

            macros_list->key = get_segment(line_buffer, 2); /*second segment is the macro name*/
            macros_list->value.string_value = malloc(1);
            if (macros_list->value.string_value == NULL)
            {
                printf("failed to allocate 1 byte");
                exit(1);
            }
            macros_list->value.string_value[0] = NULL_TERMINATOR;
            goto end_of_loop;
        }
        /*mcr exist in macros table*/
        macro_node = find_node_by_key(macros_list, segment);
        if (macro_node != NULL)
        {
            /*copy all line from macros table to file*/
            fprintf(preprocessor_output_fp, "%s", macro_node->value.string_value);

            goto end_of_loop;
        }

        /*copy line to output*/
        fprintf(preprocessor_output_fp, "%s", line_buffer);

    end_of_loop:
        free(line_buffer);
    }
    /*close output file*/
    fclose(preprocessor_output_fp);
    /* free linked list*/
    clean_linked_list(macros_list);

    return 0;
}
