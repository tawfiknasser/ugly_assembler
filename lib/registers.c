#include <stdlib.h>
#include "linked_list.h"
#include "registers.h"

/*initiate extern*/
program_status_word PSW = {0, 0, 0};

char *registers_list[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", NULL}; /* register names*/
