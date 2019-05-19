//#include "lib/debug_usart.h"
//#include "lib/f303_dac.h"
//#include "lib/dream.h"
//#include "lib/STP16_led.h"

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
