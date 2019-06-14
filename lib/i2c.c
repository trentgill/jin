#include "i2c.h"

#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>

// public defns
I2C i2c_init( char port_scl, int pin_scl
            , char port_sda, int pin_sda
            , int address
            , void (*handler)(int)
            ){
    I2C ii;
    // TODO
    return ii;
}

void i2c_send( I2C* ii, int address, int data )
{
    // TODO
}
