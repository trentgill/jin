#pragma once

#include <stm32f3xx.h>

typedef struct{
    void (*event)(int);
} I2C;

I2C i2c_init( char port_scl, int pin_scl
            , char port_sda, int pin_sda
            , int address
            , void (*handler)(int)
            );
void i2c_send( I2C* ii, int address, int data );
