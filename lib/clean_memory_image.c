#include <string.h>
#include "../globals.h"

void clean_memory_image(machine_code memory_image[])
{
    size_t i;
    for (i = 0; i < MAX_MEMORY_CAPACITY; i++)
    {
        memory_image[i].address = 0;
        memset(memory_image[i].data, 0, sizeof(memory_image[i].data));
        memory_image[i].is_symbol.value = 0;
        memset(memory_image[i].symbol_name, 0, sizeof(memory_image[i].symbol_name));
    }
}
