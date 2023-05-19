#include "../globals.h"
#include "../config.h"
#include <stdio.h>
#include <string.h>
#include "./registers.h"
#include "./check_word_type.h"

void inst_2_operands(char **words, size_t words_iterator, operations_enum operation_selector, machine_code memory_code_image[])
{
    int i, j, flag2 = 0;
    short int number;
    unsigned short int dropped_MSB;
    /*--------------------MACHINE BINARY CODE bit 13-10 (0-3)-----------------------------*/
    memory_code_image[IC].data[0].value = 0;
    memory_code_image[IC].data[1].value = 0;
    memory_code_image[IC].data[2].value = 0;
    memory_code_image[IC].data[3].value = 0;

    /*--------------------MACHINE BINARY CODE opcode 9-6 (4-7)-----------------------------*/
    for (i = 4, j = 0; i < 8; i++, j++)
    {

        if (*(operations_list[operation_selector].opcode + j) == '0')
        {
            memory_code_image[IC].data[i].value = 0;
        }
        else
        {
            memory_code_image[IC].data[i].value = 1;
        }
    }
    words_iterator++;

    /*--------------------MACHINE BINARY CODE MIUN MAKOR 5-4-(8-9)----------------------------*/

    for (i = 0; i < REGISTERS_QUANTITY; i++) /*MIUN_REGISTER 11*/
    {
        if (strcmp(words[words_iterator], registers_list[i]) == 0)
        {
            flag2 = 1;
            memory_code_image[IC].data[8].value = 1;
            memory_code_image[IC].data[9].value = 1;
        }
    }

    if ((words[words_iterator][0]) == '#' && operation_selector != 6)
    { /*MIUN_MIADI 00*/
        memory_code_image[IC].data[8].value = 0;
        memory_code_image[IC].data[9].value = 0;
    }

    else if ((words[words_iterator][0]) == '#' && operation_selector == 6)
    { /*MIUN_MIADI 00*/
        printf("\nERROR, the operend does not match the instruction.\n");
    }

    else if (flag2 == 0) /*miun yashir*/
    {
        memory_code_image[IC].data[8].value = 0;
        memory_code_image[IC].data[9].value = 1;
    }
    flag2 = 0;
    /*--------------------------------------------------------------------------------------*/
    words_iterator++; /*,*/

    if ((words[words_iterator][0]) != ',')
    {
        printf("EROR- no comma between 2 operands");
    }
    words_iterator++;

    if ((words[words_iterator]) == NULL)
    {
        printf("\nmissing operand no.2 in %s instruction\n ", words[words_iterator - 2]);
    }

    /*----------------MACHINE BINARY CODE MIUN YAAD bit 3-2-(10-11)------------------------------*/

    for (i = 0; i < REGISTERS_QUANTITY; i++) /*MIUN REGISTER 11*/
    {
        if (strcmp(words[words_iterator], registers_list[i]) == 0)
        {
            flag2 = 1;
            memory_code_image[IC].data[10].value = 1;
            memory_code_image[IC].data[11].value = 1;
        }
        else
            continue;
    }

    if ((words[words_iterator][0]) == '#' && operation_selector == 1)
    { /*MIUN_MIADI 00*/
        memory_code_image[IC].data[10].value = 0;
        memory_code_image[IC].data[11].value = 0;
    }

    else if (flag2 == 0) /*miun yashir*/
    {
        memory_code_image[IC].data[10].value = 0;
        memory_code_image[IC].data[11].value = 1;
    }
    flag2 = 0;

    /*----------------------MACHINE BINARY CODE AER bit 1-0- (12-13) ==00-------------------------------------------*/

    memory_code_image[IC].data[12].value = 0;
    memory_code_image[IC].data[13].value = 0;
    /*end of instruction*/
    /*------------------operand 1-MIUN_MAKOR-(8-9)--------------------------------------*/

    if (memory_code_image[IC].data[8].value == 0 && memory_code_image[IC].data[9].value == 0) /*MIUN_MIADI- NUMBER 12 BITS*/
    {
        IC++;
        CHECK_MEMORY_CAPACITY();

        number = atoi(words[words_iterator - 2] + 1);
        number = number << 4;
        for (i = 0; i < MACHINE_WORD_LENGTH - 2; i++)
        {
            dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
            dropped_MSB = dropped_MSB & 1;
            if (dropped_MSB == 1)
            {
                memory_code_image[IC].data[i].value = 1;
            }
            else if (dropped_MSB == 0)
            {
                memory_code_image[IC].data[i].value = 0;
            }
            number = number << 1;
        }

        memory_code_image[IC].data[12].value = 0; /*ARE 00*/
        memory_code_image[IC].data[13].value = 0; /*ARE 00*/
    }
    /*2 registers in one word*/
    else if (memory_code_image[IC].data[8].value == 1 && memory_code_image[IC].data[9].value == 1 && memory_code_image[IC].data[10].value == 1 && memory_code_image[IC].data[11].value == 1)
    {
        IC++;
        CHECK_MEMORY_CAPACITY();
        for (i = 0; i < REGISTERS_QUANTITY; i++)
        {
            if (strcmp(words[words_iterator - 2], registers_list[i]) == 0)
            {
                number = i;
                number = number << 10;

                for (i = 0; i < 6; i++)
                {
                    dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
                    dropped_MSB = dropped_MSB & 1;
                    if (dropped_MSB == 1)
                    {
                        memory_code_image[IC].data[i].value = 1;
                    }
                    else if (dropped_MSB == 0)
                    {
                        memory_code_image[IC].data[i].value = 0;
                    }
                    number = number << 1;
                }
            }
        }
    }

    else if (memory_code_image[IC].data[8].value == 1 && memory_code_image[IC].data[9].value == 1)
    { /*MIUN_REGISTER- REFITER 6 BITS*/
        IC++;
        CHECK_MEMORY_CAPACITY();
        for (i = 0; i < REGISTERS_QUANTITY; i++)
        {
            if (strcmp(words[words_iterator - 2], registers_list[i]) == 0)
            {
                number = i;
                number = number << 10;
                for (i = 0; i < MACHINE_WORD_LENGTH - 2; i++)
                {
                    dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
                    dropped_MSB = dropped_MSB & 1;
                    if (dropped_MSB == 1)
                    {
                        memory_code_image[IC].data[i].value = 1;
                    }
                    else if (dropped_MSB == 0)
                    {
                        memory_code_image[IC].data[i].value = 0;
                    }
                    number = number << 1;
                }
            }
        }
        memory_code_image[IC].data[12].value = 0; /*ARE 00*/
        memory_code_image[IC].data[13].value = 0; /*ARE 00*/
    }

    /*miun yashir-label*/
    else if (memory_code_image[IC].data[8].value == 0 && memory_code_image[IC].data[9].value == 1)
    {
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, words[words_iterator]);

        CHECK_MEMORY_CAPACITY();
    }

    /*------------------operand 2 MIUM YAAD-----------------------------------------------------*/
    /*2 registers in  command they will appear in one word ------------------------------------*/
    if (memory_code_image[IC - 1].data[8].value == 1 && memory_code_image[IC - 1].data[9].value == 1 && memory_code_image[IC - 1].data[10].value == 1 && memory_code_image[IC - 1].data[11].value == 1)
    {
        for (i = 0; i < REGISTERS_QUANTITY; i++)
        {
            if (strcmp(words[words_iterator], registers_list[i]) == 0)
            {
                number = i;
                number = number << 10;

                for (i = 6; i < 12; i++)
                {
                    dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
                    dropped_MSB = dropped_MSB & 1;
                    if (dropped_MSB == 1)
                    {
                        memory_code_image[IC].data[i].value = 1;
                    }
                    else if (dropped_MSB == 0)
                    {
                        memory_code_image[IC].data[i].value = 0;
                    }
                    number = number << 1;
                }
            }
        }
        memory_code_image[IC].data[12].value = 0; /*ARE 00*/
        memory_code_image[IC].data[13].value = 0; /*ARE 00*/
    }
    /*1 registers in  command it  will appear in one word -(10-11)-----------------------------------*/
    else if (memory_code_image[IC - 1].data[10].value == 1 && memory_code_image[IC - 1].data[11].value == 1)
    {
        IC++;
        CHECK_MEMORY_CAPACITY();
        for (i = 0; i < REGISTERS_QUANTITY; i++)

        {
            if (strcmp(words[words_iterator], registers_list[i]) == 0)
            {
                number = i;
                number = number << 4;

                for (i = 0; i < MACHINE_WORD_LENGTH - 2; i++)
                {
                    dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
                    dropped_MSB = dropped_MSB & 1;
                    if (dropped_MSB == 1)
                    {
                        memory_code_image[IC].data[i].value = 1;
                    }
                    else if (dropped_MSB == 0)
                    {
                        memory_code_image[IC].data[i].value = 0;
                    }
                    number = number << 1;
                }
            }
        }

        memory_code_image[IC].data[12].value = 0; /*ARE 00*/
        memory_code_image[IC].data[13].value = 0; /*ARE 00*/
    }
    /* a label*/
    else if (memory_code_image[IC - 1].data[10].value == 0 && memory_code_image[IC - 1].data[11].value == 1)
    {
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, words[words_iterator]);
        CHECK_MEMORY_CAPACITY();
    }

    /*--------------------------------------operator 2 -(10-11)-------------miun miadi------*/

    else if (operation_selector == CMP && memory_code_image[IC - 1].data[10].value == 0 && memory_code_image[IC - 1].data[11].value == 0)
    {
        IC++;
        CHECK_MEMORY_CAPACITY();
        number = atoi(words[words_iterator] + 1);
        number = number << 4;
        for (i = 0; i < MACHINE_WORD_LENGTH - 2; i++)
        {
            dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
            dropped_MSB = dropped_MSB & 1;
            if (dropped_MSB == 1)
            {
                memory_code_image[IC].data[i].value = 1;
            }
            else if (dropped_MSB == 0)
            {
                memory_code_image[IC].data[i].value = 0;
            }
            number = number << 1;
        }

        memory_code_image[IC].data[12].value = 0; /*ARE 00*/
        memory_code_image[IC].data[13].value = 0; /*ARE 00*/
    }

    if (words[words_iterator + 1] != NULL)
    {
        printf("ERROR: too much arguments\n");
    }

    IC++;
    CHECK_MEMORY_CAPACITY();
    return;
}
