#pragma once

#include <stm32f3xx.h>

typedef struct{
    GPIO_TypeDef* gpio;
    uint16_t      pin;
    int           state;
} dout_t;

dout_t dout_init(void);
void dout_set( dout_t* out, int state );
void dout_flip( dout_t* out );
