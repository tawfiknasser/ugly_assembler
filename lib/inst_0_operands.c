#include "../globals.h"
#include "../config.h"
#include <stdio.h>
void inst_0_operands(char **words, size_t words_iterator, operations_enum operation_selector, machine_code memory_code_image[])
{
    int i, j;
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

    /*--------------------MACHINE BINARY CODE MIUN MAKOR 5-4-(8-9) always 00----------------------------*/

    memory_code_image[IC].data[8].value = 0;
    memory_code_image[IC].data[9].value = 0;

    /*------------------MACHINE BINARY CODE MIUN YAAD bit 3-2-(10-11) always 00------------------------------*/

    memory_code_image[IC].data[10].value = 0;
    memory_code_image[IC].data[11].value = 0;

    /*----------------------MACHINE BINARY CODE AER bit 1-0- (12-13) ==00-------------------------------------------*/

    memory_code_image[IC].data[12].value = 0;
    memory_code_image[IC].data[13].value = 0;

    if (words[words_iterator + 1] != NULL)
    {
        printf("There is more operands than expected for this command\n");
    }

    IC++;
    CHECK_MEMORY_CAPACITY();

    return;
}
