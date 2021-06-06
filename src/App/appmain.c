#include "main.h"

#include "tx_api.h"
#include "utils.h"
#include "demo.h"
#include "mem_layout.h"


int main(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
    MX_All_Init();
    LOG("begin trace\n")

    LOG("begin kernel\n")
    tx_kernel_enter();

    while (1)
    {

    }
}
