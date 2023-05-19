#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../globals.h"
#include "../config.h"

void data_handler(char **words, size_t words_iterator, machine_code memory_data_image[])
{

    short int number, i;
    unsigned int dropped_MSB;
    char *p;

    while (words[words_iterator] != NULL)
    {

        if (words[words_iterator][0] == COMMA)
        {
            printf("Error: Multiple consecutive commas.\n");
            words_iterator++;
            continue;
        }

        strtol(words[words_iterator], &p, 10);
        if (
            ((words[words_iterator][0] == PLUS) || (words[words_iterator][0] == MINUS) ||
             (isdigit(words[words_iterator][0]))) &&
            ((*p) == '\0') && (strchr(words[words_iterator], '.') == NULL))
        {
            number = atoi(words[words_iterator]);
            number = number << 2;
            for (i = 0; i < MACHINE_WORD_LENGTH; i++)
            {

                dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
                dropped_MSB = dropped_MSB & 1;
                if (dropped_MSB == 1)
                {
                    memory_data_image[DC].data[i].value = 1;
                }
                else if (dropped_MSB == 0)
                {
                    memory_data_image[DC].data[i].value = 0;
                }
                number = number << 1;
            }
            words_iterator++;
            DC++;
            CHECK_MEMORY_CAPACITY();
        }

        if (words[words_iterator] != NULL && (words[words_iterator][0] == COMMA))
        {
            words_iterator++;
            continue;
        }
        else if (words[words_iterator] == NULL)
        {
            break;
        }
        else
        {
            printf("Error: data argument is not correct or missing comma between 2 arguments.\n");
            words_iterator++;
            continue;
        }
    }
    /*end of while*/

    return; /*end of the function*/
}