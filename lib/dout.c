#include "dout.h"

#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>

// private declarations
static inline void dout_write( Dout* o );

// public defns
Dout* dout_init(void)
{
// debug led hardcoded
    __HAL_RCC_GPIOB_CLK_ENABLE();

    Dout* d = malloc( sizeof( Dout ));
    if( !d ){ printf("dout malloc!\n"); return NULL; }
    d->gpio  = GPIOB;
    d->pin   = GPIO_PIN_4;
    d->state = 0;
    //Dout d = { .gpio  = GPIOB
    //           , .pin   = GPIO_PIN_4
    //           , .state = 0
    //           };

    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Pin  = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOB, &gpio);

    return d;
    //return &d;
}

void dout_set( Dout* out, int state )
{
    out->state = (state) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    dout_write( out );
}

void dout_flip( Dout* out )
{
    out->state ^= 1;
    dout_write( out );
}

// private defns
static inline void dout_write( Dout* o ){
    HAL_GPIO_WritePin( o->gpio
                     , o->pin
                     , o->state
                     );
}
