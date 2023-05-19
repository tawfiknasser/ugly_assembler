#include "../globals.h"
#include "../lib/linked_list.h"
#include "../lib/read_line.h"
#include "../lib/inst_1_operands.h"
#include "../lib/inst_2_operands.h"
#include "../lib/build_words_array.h"

int assembler_second_pass(node **symbol_table, machine_code memory_code_image[])
{
    int RC = 0, i, IC_CLONE = IC;
    short int number;
    unsigned int dropped_MSB;
    node *symbol = NULL;

    while (IC_CLONE > 0)
    {
        if (memory_code_image[IC_CLONE].is_symbol.value && memory_code_image[IC_CLONE].is_symbol.value == 1)
        {
            symbol = find_node_by_key(*symbol_table, memory_code_image[IC_CLONE].symbol_name);

            if (symbol == NULL)
            {
                printf("Error: %s symbol does not exist in the symbol table\n", memory_code_image[IC_CLONE].symbol_name);
                RC++;
                IC_CLONE--;
                continue;
            }
            number = symbol->value.int_value;

            number = number << 4;
            for (i = 0; i < MACHINE_WORD_LENGTH - 2; i++)
            {
                dropped_MSB = number >> (sizeof(short int) * BYTE_LENGTH - 1);
                dropped_MSB = dropped_MSB & 1;
                if (dropped_MSB == 1)
                {
                    memory_code_image[IC_CLONE].data[i].value = 1;
                }
                else if (dropped_MSB == 0)
                {
                    memory_code_image[IC_CLONE].data[i].value = 0;
                }
                number = number << 1;
            }

            /*AER for label*/
            if (symbol->node_type == (1 << 2))
            { /*extern*/
                memory_code_image[IC_CLONE].data[12].value = 0;
                memory_code_image[IC_CLONE].data[13].value = 1;
            }
            else
            {
                memory_code_image[IC_CLONE].data[12].value = 1;
                memory_code_image[IC_CLONE].data[13].value = 0;
            }
        }
        IC_CLONE--;
    }

    return RC;
}