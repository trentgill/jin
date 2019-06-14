#pragma once

#include <stm32f3xx.h>

GPIO_TypeDef* gpio_enable( char gpio );
uint16_t gpio_get_pin( int pin );
int gpio_from_pin( uint16_t pin );
