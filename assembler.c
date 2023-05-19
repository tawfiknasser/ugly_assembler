#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "config.h"
#include "assembler.h"
#include "lib/linked_list.h"
#include "lib/open_file.h"
#include "lib/clean_memory_image.h"
#include "core/preprocessor.h"
#include "core/first_pass.h"
#include "core/second_pass.h"

/*initiate globals*/
size_t IC = 0;
size_t DC = 0;
operation operations_list[NUM_OF_OPERATIONS] =

    {
        {"mov", "0000"},
        {"cmp", "0001"},
        {"add", "0010"},
        {"sub", "0011"},
        {"not", "0100"},
        {"clr", "0101"},
        {"lea", "0110"},
        {"inc", "0111"},
        {"dec", "1000"},
        {"jmp", "1001"},
        {"bne", "1010"},
        {"red", "1011"},
        {"prn", "1100"},
        {"jsr", "1101"},
        {"rts", "1110"},
        {"stop", "1111"}};

size_t memory_capacity_counter = 0;

/*
* This function "assemble" is responsible running the whole assembling flow on all source files.
* This diagram explains the flow on each source file:
* Open file --> Pre-process (expands macros) --> First pass --> Second pass --> Write output files
*/
int assemble(int queue_size, char **files_queue)
{
    FILE *source_file;
    int i, j, k, rc, entry_exist_flag = 0;
    char *tmp_file_name;
    node *symbol;
    node *symbol_table = NULL;
    machine_code memory_data_image[MAX_MEMORY_CAPACITY]; /* to store the data memory image. MAX_MEMORY_CAPACITY is as defined in the project requirments*/
    machine_code memory_code_image[MAX_MEMORY_CAPACITY]; /* to store the instruction memory image*/

    if (!queue_size)
        return 0;

    for (i = 1; i < queue_size; i++)
    {
        /*Start of Pre-process (expands macros)*/
        {
            /*tmp_file_name to be used to concat with extensions when reading*/
            tmp_file_name = (char *)malloc(sizeof(char) * (strlen(files_queue[i]) + 1 + strlen(SOURCE_EXTENSION)));
            strcpy(tmp_file_name, files_queue[i]);
            strcat(tmp_file_name, SOURCE_EXTENSION);

            source_file = open_file(tmp_file_name, "r");
            if (source_file == NULL)
            {
                free(tmp_file_name);
                continue;
            }

            /*expands macros, ignore comments and empty lines*/
            preprocessor(source_file, files_queue[i]);
            fclose(source_file);
            free(tmp_file_name);
       /*End of Pre-process (expands macros)*/}

       { /*Start of first pass*/
           IC = 0;
           DC = 0;
           memory_capacity_counter = 0;
           symbol_table = NULL;

           tmp_file_name = (char *)malloc(sizeof(char) * (strlen(files_queue[i]) + 1 + strlen(PREPROCESS_EXTENSION)));
           strcpy(tmp_file_name, files_queue[i]);
           strcat(tmp_file_name, PREPROCESS_EXTENSION);
           source_file = open_file(tmp_file_name, "r");
           if (source_file == NULL)
           {
               free(tmp_file_name);
               continue;
           }

           /*First file pass. build the symbol table, data and code memory. */
           rc = assembler_first_pass(source_file, &symbol_table, memory_data_image, memory_code_image);
           fclose(source_file);
           free(tmp_file_name);
           /*stop assembling/processing this file if there is errors */
           if (rc)
               goto end_of_file_process;

           /*build address*/
           for (j = 0; j < IC; j++)
               memory_code_image[j].address = j + MEMORY_START_ADDRESS;

           for (j = 0; j < DC; j++)
               memory_data_image[j].address = j + IC + MEMORY_START_ADDRESS;

           symbol = symbol_table;

           while (symbol != NULL)
           {
               if ((symbol->node_type & 1) == 1)
                   symbol->value.int_value += IC + MEMORY_START_ADDRESS;

               if ((symbol->node_type & (1 << 1)) == (1 << 1))
                   symbol->value.int_value += MEMORY_START_ADDRESS;

               symbol = symbol->next;
           }
        /*End of first pass*/}

       { /*Start of second pass*/
           rc = assembler_second_pass(&symbol_table, memory_code_image);
           if (rc)
               goto end_of_file_process;

        /*End of second pass*/}

       /*start write output files*/
       /*write object file*/
       tmp_file_name = (char *)malloc(sizeof(char) * (strlen(files_queue[i]) + 1 + strlen(PREPROCESS_EXTENSION)));
       strcpy(tmp_file_name, files_queue[i]);
       strcat(tmp_file_name, OBJECT_EXTENSION);
       source_file = open_file(tmp_file_name, "w");
       if (source_file == NULL)
       {
           free(tmp_file_name);
           continue;
       }

       fprintf(source_file, "              %d  %d  \n", (int)IC, (int)DC);
       for (j = 0; j < IC; j++)
       {
           k = 0;
           fprintf(source_file, "%u       ", memory_code_image[j].address);
           while (k < 14)
           {
               fprintf(source_file, "%c", memory_code_image[j].data[k].value ? '/' : '.');
               k++;
           }
           fprintf(source_file, "\n");
       }
       fprintf(source_file, "\n"); /*seperate */
       for (j = 0; j < DC; j++)
       {
           k = 0;
           fprintf(source_file, "%u       ", memory_data_image[j].address);
           while (k < 14)
           {
               fprintf(source_file, "%c", memory_data_image[j].data[k].value ? '/' : '.');
               k++;
           }
           fprintf(source_file, "\n");
       }
       fclose(source_file);

       /*write entries files*/
       entry_exist_flag = 0;
       tmp_file_name = (char *)malloc(sizeof(char) * (strlen(files_queue[i]) + 1 + strlen(ENTRIES_EXTENSION)));
       strcpy(tmp_file_name, files_queue[i]);
       strcat(tmp_file_name, ENTRIES_EXTENSION);
       source_file = open_file(tmp_file_name, "w");
       if (source_file == NULL)
       {
           free(tmp_file_name);
           continue;
       }

       symbol = symbol_table;
       while (symbol != NULL)
       {
           if ((((symbol->node_type) >> 3) & 1) == 1)
           {
               entry_exist_flag = 1;
               fprintf(source_file, "%s %d\n", symbol->key, symbol->value.int_value); /*seperate */
           }

           symbol = symbol->next;
       }
       fclose(source_file);
       if (entry_exist_flag == 0)
           remove(tmp_file_name);

       /*write extern file*/
       entry_exist_flag = 0;
       tmp_file_name = (char *)malloc(sizeof(char) * (strlen(files_queue[i]) + 1 + strlen(EXTERNAL_EXTENSION)));
       strcpy(tmp_file_name, files_queue[i]);
       strcat(tmp_file_name, EXTERNAL_EXTENSION);
       source_file = open_file(tmp_file_name, "w");
       if (source_file == NULL)
       {
           free(tmp_file_name);
           continue;
       }

       for (j = 0; j < IC; j++)
       {

           if (memory_code_image[j].is_symbol.value == 1)
           {
               symbol = find_node_by_key(symbol_table, memory_code_image[j].symbol_name);
               if (symbol == NULL)
                   printf("Error: call the police.\n");

               if ((((symbol->node_type) >> 2 & 1) == 1))
               {
                   entry_exist_flag = 1;
                   fprintf(source_file, "%s %u\n", symbol->key, memory_code_image[j].address);
               }
           }
       }

       fclose(source_file);
       if (entry_exist_flag == 0)
           remove(tmp_file_name);

    end_of_file_process:
        /*free symbole table*/
        clean_linked_list(symbol_table);
        /*clean memory_data_image and memory_code_image*/
        clean_memory_image(memory_code_image);
        clean_memory_image(memory_data_image);
    }

    printf("\nThank you message\n");
    return 0;
}

/* UNIT_TEST to be used if running other main function is required by unit tests*/
#ifndef UNIT_TEST
/*
* The first argument is the file name. 
* the other arguments expected to be source files paths without extension
*/
int main(int argc, char **argv)
{
    return assemble(argc, argv);
}
#endif
