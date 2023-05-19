#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../globals.h"
#include "../config.h"
#include "../lib/linked_list.h"
#include "../lib/registers.h"
#include "../lib/get_segment.h"
#include "../lib/read_line.h"
#include "../lib/check_word_type.h"
#include "../lib/data_handler.h"
#include "../lib/string_handler.h"
#include "../lib/inst_0_operands.h"
#include "../lib/inst_1_operands.h"
#include "../lib/inst_2_operands.h"
#include "../lib/build_words_array.h"

int assembler_first_pass(FILE *source_file, node **symbol_table, machine_code memory_data_image[], machine_code memory_code_image[])
{
    int RC = 0;
    operations_enum operation_selector;
    char *line_buffer = NULL;
    char **words_array;
    size_t words_iterator = 0;
    word_type curr_word_type;
    char label_name[MAX_LABEL_LENGTH] = {'\0'};
    node *symbol = NULL;

    while ((line_buffer = read_line(source_file)) != NULL)
    {
        PSW.IS_SYMBOLE_DEFINE = 0;
        words_iterator = 0;
        words_array = build_words_array(line_buffer);

        /*check if first word is label*/
        curr_word_type = check_word_type(words_array[0]);
        if (curr_word_type == ERROR_WORD)
        {
            /*on error increase the RC and continue to the next line*/
            RC++;
            goto end_of_loop;
        }
        if (curr_word_type == LABEL)
        {
            PSW.IS_SYMBOLE_DEFINE |= 1;
            words_iterator++;
            curr_word_type = check_word_type(words_array[words_iterator]);
        }

        switch (curr_word_type)
        {
        case DATA:
            words_iterator++;
            if (PSW.IS_SYMBOLE_DEFINE == 1)
            {

                strncpy(label_name, words_array[0], strlen(words_array[0]) - 1);
                label_name[strlen(words_array[0]) - 1] = NULL_TERMINATOR;
                symbol = find_node_by_key(*symbol_table, label_name);
                if (symbol == NULL)
                {

                    *symbol_table = add_node_to_head(*symbol_table);

                    (*symbol_table)->key = malloc(strlen(label_name));
                    (*symbol_table)->key[0] = '\0';
                    strcpy((*symbol_table)->key, label_name);

                    (*symbol_table)->value.int_value = DC;
                    (*symbol_table)->node_type |= 1;

                    data_handler(words_array, words_iterator, memory_data_image);

                    break;
                }
                else if (symbol->node_type == (1 << 3))
                {
                    symbol->value.int_value = DC;
                    symbol->node_type |= 1;
                    data_handler(words_array, words_iterator, memory_data_image);
                }
                else
                {
                    printf("ERROR: the label %s has already appeared in the symboleTable\n", words_array[0]);
                    break;
                }
            }
            else
            {
                data_handler(words_array, words_iterator, memory_data_image);
            }
            break;
        case STRING:
            words_iterator++;
            if (PSW.IS_SYMBOLE_DEFINE != 1)
            {
                string_handler(words_array, words_iterator, memory_data_image);
                break;
            }

            if (find_node_by_key(*symbol_table, words_array[0]))
            {
                printf("ERROR: the label %s has already appeared in the symboleTable\n", words_array[0]);
                break;
            }

            symbol = find_node_by_key(*symbol_table, words_array[0]);
            if (symbol == NULL)
            {
                *symbol_table = add_node_to_head(*symbol_table);
                strncpy(label_name, words_array[0], strlen(words_array[0]) - 1);
                label_name[strlen(words_array[0]) - 1] = NULL_TERMINATOR;
                (*symbol_table)->key = malloc(strlen(label_name));
                (*symbol_table)->key[0] = '\0';

                strcpy((*symbol_table)->key, label_name);
                symbol = *symbol_table;
            }

            symbol->value.int_value = DC;
            symbol->node_type |= 1; /*data flag*/

            string_handler(words_array, words_iterator, memory_data_image);

            break;

        case ENT:
            /*handling entries in the second pass*/
            if (PSW.IS_SYMBOLE_DEFINE == 1)
            {
                printf("Warning: No label should be defined for entry\n");
                words_iterator++;
            }

            strncpy(label_name, words_array[words_iterator + 1], strlen(words_array[words_iterator + 1]));
            label_name[strlen(words_array[words_iterator + 1])] = NULL_TERMINATOR;
            symbol = find_node_by_key(*symbol_table, label_name);
            if (symbol == NULL)
            {
                *symbol_table = add_node_to_head(*symbol_table);
                (*symbol_table)->key = malloc(strlen(label_name));
                (*symbol_table)->key[0] = '\0';
                strcpy((*symbol_table)->key, label_name);

                (*symbol_table)->value.int_value = 0;
                (*symbol_table)->node_type |= (1 << 3); /* entry flag*/
            }
            else
            {
                symbol->node_type |= (1 << 3);
            }

            break;
        case EXT:
            if (PSW.IS_SYMBOLE_DEFINE == 1)
            {
                printf("Warning: No label should be defined for external\n");
                words_iterator++;
            }
            strncpy(label_name, words_array[words_iterator + 1], strlen(words_array[words_iterator + 1]));
            label_name[strlen(words_array[words_iterator + 1])] = NULL_TERMINATOR;
            symbol = find_node_by_key(*symbol_table, label_name);
            if (symbol == NULL)
            {
                *symbol_table = add_node_to_head(*symbol_table);
                strncpy(label_name, words_array[words_iterator + 1], strlen(words_array[words_iterator + 1]));
                label_name[strlen(words_array[words_iterator + 1])] = NULL_TERMINATOR;
                (*symbol_table)->key = malloc(strlen(label_name));
                (*symbol_table)->key[0] = '\0';
                strcpy((*symbol_table)->key, label_name);

                symbol = *symbol_table;
            }
            symbol->value.int_value = 0;
            symbol->node_type |= (1 << 2); /* extern flag*/

            break;
        case INST:
            /*code image handling*/
            if (PSW.IS_SYMBOLE_DEFINE == 1)
            {
                strncpy(label_name, words_array[0], strlen(words_array[0]) - 1);
                label_name[strlen(words_array[0]) - 1] = NULL_TERMINATOR;
                if (find_node_by_key(*symbol_table, label_name))
                {
                    printf("ERROR: the label %s has already appeared in the symboleTable in handling code.\n", words_array[0]);
                    break;
                }

                symbol = find_node_by_key(*symbol_table, words_array[0]);
                if (symbol == NULL)
                {
                    *symbol_table = add_node_to_head(*symbol_table);

                    (*symbol_table)->key = malloc(strlen(label_name));
                    (*symbol_table)->key[0] = '\0';
                    strcpy((*symbol_table)->key, label_name);
                    symbol = *symbol_table;
                }

                symbol->value.int_value = IC;
                symbol->node_type |= (1 << 1); /*code flag*/
            }

            /*find operation*/
            for (operation_selector = 0; operation_selector < NUM_OF_OPERATIONS;)
            {
                if (strcmp(operations_list[operation_selector].name, words_array[words_iterator]) == 0)
                {
                    break; /*operation found. stop loop.*/
                }
                operation_selector++;
            }

            if (operation_selector == MOV || operation_selector == CMP || operation_selector == ADD || operation_selector == SUB || operation_selector == LEA)
            {
                inst_2_operands(words_array, words_iterator, operation_selector, memory_code_image);
                break;
            }
            else if (operation_selector == NOT || operation_selector == CLR || operation_selector == INC || operation_selector == DEC || operation_selector == JMP || operation_selector == BNE || operation_selector == RED || operation_selector == PRN || operation_selector == JSR)
            {
                inst_1_operands(words_array, words_iterator, operation_selector, memory_code_image);
                break;
            }
            else if (operation_selector == RTS || operation_selector == STOP)
            {
                inst_0_operands(words_array, words_iterator, operation_selector, memory_code_image);
                break;
            }

            break;
        default:
            break;
        }

    end_of_loop:
        free(words_array);
    }

    return RC;
}
