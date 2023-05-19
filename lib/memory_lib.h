#ifndef MEMORY_LIB_H
#define MEMORY_LIB_H

enum AER
{
    ABSOLUTE = 0,
    EXTERNAL,
    RELOCATABLE
};

typedef struct immediate
{
    unsigned int AER : 2; /*Absolute, External, Relocatable*/
    unsigned int value : 12;
} immediate;

typedef struct instruction
{
    unsigned int AER : 2; /*Absolute, External, Relocatable*/
    unsigned int operand_dest : 2;
    unsigned int operand_source : 2;
    unsigned int operation_code : 4;
    unsigned int param_1 : 2;
    unsigned int param_2 : 2;
} instruction;

typedef struct memory_word
{

    union word {
        instruction *instruction_code;
        immediate *immediate_code;
    }

} memory_word;

typedef struct data_image
{

} data_image;

#endif