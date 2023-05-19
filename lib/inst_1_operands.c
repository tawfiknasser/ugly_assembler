#include "../globals.h"
#include "../config.h"
#include <stdio.h>
#include <string.h>
#include "./registers.h"
#include "./check_word_type.h"

void inst_1_operands(char **words, size_t words_iterator, operations_enum operation_selector, machine_code memory_code_image[])
{
    int i, j, flag = 0, flag1 = 0, flag2 = 0;
    char *p, *q, *t, *z;
    short int number;
    char word4[31];
    unsigned short int dropped_MSB;

    /*--------------------MACHINE BINARY CODE bit 13-10 (0-3)-----------------------------*/
    /*will be determaned later*/
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

    /*--------------------MACHINE BINARY CODE MIUN MAKOR 5-4-(8-9) always 00----------------------------*/

    memory_code_image[IC].data[8].value = 0;
    memory_code_image[IC].data[9].value = 0;

    /*--------------------------------------------------------------------------------------*/

    words_iterator++; /*second operand*/

    strcpy(word4, words[words_iterator]);

    /*----------------MACHINE BINARY CODE MIUN YAAD bit 3-2-(10-11)------------------------------*/

    for (i = 0; i < REGISTERS_QUANTITY; i++) /*MIUN REGISTER 11*/
    {
        if (strcmp(words[words_iterator], registers_list[i]) == 0)
        {
            flag = 1;
            memory_code_image[IC].data[10].value = 1;

            memory_code_image[IC].data[11].value = 1;
        }
        else
            continue;
    }

    if (operation_selector == PRN && flag == 0)
    {

        if ((words[words_iterator][0]) == '#')
        { /*MIUN_MIADI 00*/
            memory_code_image[IC].data[10].value = 0;
            memory_code_image[IC].data[11].value = 0;
        }
    }

    else if ((operation_selector == JMP || operation_selector == BNE || operation_selector == JSR) && flag == 0 && (((p = strchr(words[words_iterator], '(')) != NULL) && ((q = strchr(words[words_iterator], ')')) != NULL))) /*miun kfitza 10*/
    {

        memory_code_image[IC].data[10].value = 1;
        memory_code_image[IC].data[11].value = 0;

        t = strtok(word4, "(");
        t = strtok(NULL, ","); /*parameter 1*- we need to put the label in the tablelist*/

        for (i = 0; i < REGISTERS_QUANTITY; i++)
        {

            if (strcmp(t, registers_list[i]) == 0) /*parametr 1 is a register-11*/
            {
                flag1 = 1;
                memory_code_image[IC].data[0].value = 1;
                memory_code_image[IC].data[1].value = 1;
            }
        }
        if ((*t) == '#' && flag1 == 0)
        { /*parametr 1 is a number-00*/
            memory_code_image[IC].data[0].value = 0;
            memory_code_image[IC].data[1].value = 0;
        }

        else if (flag1 == 0) /*parameter1 is label- 01*/
        {

            memory_code_image[IC].data[0].value = 0; /*parametr 1 is a label 01*/
            memory_code_image[IC].data[1].value = 1;
        }

        z = strtok(NULL, ")"); /*parameter 2*/

        for (i = 0; i < REGISTERS_QUANTITY; i++)
        {
            if (strcmp(z, registers_list[i]) == 0) /*parametr 2 is a register-11*/
            {
                flag2 = 1;
                memory_code_image[IC].data[2].value = 1;
                memory_code_image[IC].data[3].value = 1;
            }
        }

        if ((*z) == '#' && flag2 == 0) /*parametr 2is a number-00*/
        {
            memory_code_image[IC].data[2].value = 0;
            memory_code_image[IC].data[3].value = 0;
        }
        else if (flag2 == 0) /*parametr 2is a label 01*/
        {
            memory_code_image[IC].data[2].value = 0; /*parametr 1 is a label 01*/
            memory_code_image[IC].data[3].value = 1;
        }
    }
    /*operand 2 is miun yashir-label 01 */
    else if (flag == 0 && is_legal_label(words[words_iterator]) == 1)
    {
        memory_code_image[IC].data[10].value = 0;
        memory_code_image[IC].data[11].value = 1;
    }
    else
        printf("\nEROOR, there is no match bwtween instruction and opernands");

    /*----------------------MACHINE BINARY CODE AER bit 1-0- (12-13) ==00-------------------------------*/
    memory_code_image[IC].data[12].value = 0;
    memory_code_image[IC].data[13].value = 0;
    /*end of instruction */
    /*------------------operand 2---------------------------------*/
    /*OPERAND 2 MIUN MIADI 00*/

    if (memory_code_image[IC].data[10].value == 0 && memory_code_image[IC].data[11].value == 0) /*MIUN_MIADI- NUMBER 12 BITS*/
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
        IC++;
        CHECK_MEMORY_CAPACITY();

        return;
    }

    /*OPERAND 2 IS A register ------------------------------------*/
    if (memory_code_image[IC].data[10].value == 1 && memory_code_image[IC].data[11].value == 1)
    {

        IC++;
        CHECK_MEMORY_CAPACITY();

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
        IC++;
        CHECK_MEMORY_CAPACITY();

        return;
    }
    /*operand 2 is a label*/
    else if (memory_code_image[IC].data[10].value == 0 && memory_code_image[IC].data[11].value == 1)
    {
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, strtok(words[words_iterator], "("));
        IC++;
        CHECK_MEMORY_CAPACITY();
        return;
    }

    /*OPERAND 2 IS MIUN KFITZA 2 registers in  command  will appear in one word -(10-11)-----------------------------------*/

    else if (memory_code_image[IC].data[0].value == 1 && memory_code_image[IC].data[1].value == 1 && memory_code_image[IC].data[2].value == 1 &&
             memory_code_image[IC].data[3].value == 1)

    {

        IC++; /*place for the label in niun kfitza*/
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, strtok(words[words_iterator], "("));

        IC++; /*place for 2 register word*/
        CHECK_MEMORY_CAPACITY();

        for (i = 0; i < REGISTERS_QUANTITY; i++)

        {
            if (strcmp(t, registers_list[i]) == 0)
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
        for (i = 0; i < REGISTERS_QUANTITY; i++)
        {
            if (strcmp(z, registers_list[i]) == 0)
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
            memory_code_image[IC].data[12].value = 0; /*ARE 00*/

            memory_code_image[IC].data[13].value = 0; /*ARE 00*/
        }
    }
    /*miun kfita- operator 1 is only a register */
    else if (memory_code_image[IC].data[0].value == 1 && memory_code_image[IC].data[1].value == 1)
    {
        IC++; /*place for the label in miun kfitza*/
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, strtok(words[words_iterator], "("));
        IC++;
        CHECK_MEMORY_CAPACITY();

        for (i = 0; i < REGISTERS_QUANTITY; i++)

        {
            if (strcmp(t, registers_list[i]) == 0)
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
    /*miun kfizta- operator 1 - a number*/
    else if (memory_code_image[IC].data[0].value == 0 && memory_code_image[IC].data[1].value == 0)
    {
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, strtok(words[words_iterator], "("));
        IC++;
        CHECK_MEMORY_CAPACITY();

        number = atoi(t + 1);
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
    else if (memory_code_image[IC].data[0].value == 0 && memory_code_image[IC].data[1].value == 1) /*a label*/
    {
        /*place for label- miun kfitza*/
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, strtok(words[words_iterator], "("));
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, t);
        CHECK_MEMORY_CAPACITY();
    }

    /*operator 2 in miun kfitza*/
    if (memory_code_image[IC - 2].data[0].value != 1 &&
        memory_code_image[IC - 2].data[2].value == 1 && memory_code_image[IC - 2].data[3].value == 1) /*register*/
    {
        IC++;
        CHECK_MEMORY_CAPACITY();

        for (i = 0; i < REGISTERS_QUANTITY; i++)

        {
            if (strcmp(z, registers_list[i]) == 0)
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
    }
    else if (memory_code_image[IC - 2].data[2].value == 0 && memory_code_image[IC - 2].data[3].value == 0) /*number*/
    {

        IC++;
        CHECK_MEMORY_CAPACITY();

        number = atoi(z + 1);
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
    else if (memory_code_image[IC - 2].data[2].value == 0 && memory_code_image[IC - 2].data[3].value == 1)
    {
        IC++;
        memory_code_image[IC].is_symbol.value = 1;
        strcpy(memory_code_image[IC].symbol_name, z);
    }

    IC++;
    CHECK_MEMORY_CAPACITY();

    return;
}
