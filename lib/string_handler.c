#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../globals.h"
#include "../config.h"

void string_handler(char **words, size_t words_iterator, machine_code memory_data_image[])
{
    int content_length = strlen(words[words_iterator]) - 2; /*  string length without " " */
    int string_iterator = 1, i, number;
    unsigned int dropped_MSB;

    /*validate syntax*/
    if (words[words_iterator][0] != '"' || words[words_iterator][strlen(words[words_iterator]) - 1] != '"')
    {
        printf("Error: not valid syntax for .string");
        return;
    }

    while (string_iterator < content_length + 1)
    {
        number = (words[words_iterator][string_iterator]);
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
        string_iterator++;
        DC++;
        CHECK_MEMORY_CAPACITY();
    }

    /* putting the '\0' as a new word at the end data arr like they asked*/
    for (i = 0; i < MACHINE_WORD_LENGTH; i++)
        memory_data_image[DC].data[i].value = 0;

    string_iterator++;

    DC++;
    CHECK_MEMORY_CAPACITY();

    return;
}