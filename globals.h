#ifndef GLOBALS_H
#define GLOBALS_H
#include <stdlib.h>
#include "./config.h"
extern size_t IC, DC;

#define NUM_OF_OPERATIONS 16
typedef struct
{
    char *name;
    char *opcode;
} operation;
extern operation operations_list[NUM_OF_OPERATIONS];

#define MEMORY_START_ADDRESS 100
typedef enum word_type
{
    UNDEFINED,
    LABEL,
    DATA,
    STRING,
    ENT,
    EXT,
    INST,
    ERROR_WORD
} word_type;

#define TRUE 1
#define FALSE 0

typedef struct bit
{
    unsigned int value : 1;
} bit;

#define MACHINE_WORD_LENGTH 14
#define BYTE_LENGTH 8
typedef struct machine_code
{
    unsigned int address;
    bit data[MACHINE_WORD_LENGTH];

    bit is_symbol; /*0 1*/
    char symbol_name[MAX_LABEL_LENGTH];

} machine_code;

extern size_t memory_capacity_counter;

#define MAX_MEMORY_CAPACITY 256

#define CHECK_MEMORY_CAPACITY()                      \
    do                                               \
    {                                                \
        if ((DC) + (IC) + 1 > (MAX_MEMORY_CAPACITY)) \
        {                                            \
            printf("Exceeded MAX_MEMORY_CAPACITY");  \
            exit(1);                                 \
        }                                            \
    } while (0)

typedef enum operations_enum
{
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP
} operations_enum;

#endif