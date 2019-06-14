#include "gpio.h"

#include <stdio.h>

// public defns
GPIO_TypeDef* gpio_enable( char gpio )
{
    // TODO query if it should be broken up into 'enable' and 'get_gpio'
    GPIO_TypeDef* g;
    switch(gpio){
        case 'a': case 'A': __HAL_RCC_GPIOA_CLK_ENABLE(); g = GPIOA; break;
        case 'b': case 'B': __HAL_RCC_GPIOB_CLK_ENABLE(); g = GPIOB; break;
        case 'c': case 'C': __HAL_RCC_GPIOC_CLK_ENABLE(); g = GPIOC; break;
        case 'd': case 'D': __HAL_RCC_GPIOD_CLK_ENABLE(); g = GPIOD; break;
        case 'f': case 'F': __HAL_RCC_GPIOF_CLK_ENABLE(); g = GPIOF; break;
        default: printf("pin not found\n"); g = NULL;
    }
    return g;
}

uint16_t gpio_get_pin( int pin )
{
    return (1 << pin);
}

int gpio_from_pin( uint16_t pin )
{
    for( int i=0; i<16; i++ ){
        if( pin & (1 << i) ){ return i; }
    }
    return -1;
}
