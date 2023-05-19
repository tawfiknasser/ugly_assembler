#ifndef REGISTERS_H
#define REGISTERS_H

#define REGISTERS_QUANTITY 8

typedef struct
{
    unsigned int Z : 1;
    unsigned int IS_MCR : 1;
    unsigned int IS_SYMBOLE_DEFINE : 1;

} program_status_word;

/*program status word*/
extern program_status_word PSW;
extern char *registers_list[];

#endif