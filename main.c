#include "lib/system.h"
#include "lib/dout.h"

// private functions
int main(void)
{
    STM32_Init();

    dout_t led = dout_init();

    while (1){
        dout_set(&led,1);
    }
}
