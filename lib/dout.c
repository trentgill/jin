#include "dout.h"

#include <stm32f3xx.h>

// private declarations
static inline void dout_write( dout_t* o );

// public defns
dout_t dout_init(void)
{
// debug led hardcoded
    __HAL_RCC_GPIOB_CLK_ENABLE();

    dout_t d = { .gpio  = GPIOB
               , .pin   = GPIO_PIN_4
               , .state = 0
               };

    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Pin  = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOB, &gpio);

    return d;
}

void dout_set( dout_t* out, int state )
{
    out->state = (state) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    dout_write( out );
}

void dout_flip( dout_t* out )
{
    out->state ^= 1;
    dout_write( out );
}

// private defns
static inline void dout_write( dout_t* o ){
    HAL_GPIO_WritePin( o->gpio
                     , o->pin
                     , o->state
                     );
}
