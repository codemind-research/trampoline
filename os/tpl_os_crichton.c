#include "tpl_os_crichton.h"

void crichton_free(void* ptr)
{
    printf("Wrap version free\r\n");
    free(ptr);
}
