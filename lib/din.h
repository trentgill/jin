#pragma once

#include <stm32f3xx.h>

typedef struct{
    GPIO_TypeDef*  port;
    int            pin;
    void(*handler)(int);
} Din;

Din din_init( char gpio, int pin );
Din din_event( char gpio, int pin, void (*handler)(int) );
int din_get( Din* in );
