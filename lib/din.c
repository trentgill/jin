#include "din.h"

#include <stdio.h>
#include <stdlib.h>

#include "gpio.h"

void(*events[16])(int,int); // limited to 1-event per exti
GPIO_TypeDef* e_port[16];

// private declarations
static int din_get_irq( int pin );

// public defns
Din din_init( char gpio, int pin )
{
    Din din = { .port = gpio_enable( gpio )
              , .pin  = gpio_get_pin( pin )
              };
    GPIO_InitTypeDef g = { .Mode = GPIO_MODE_INPUT
                         , .Pull = GPIO_PULLUP
                         , .Pin  = din.pin
                         };
    HAL_GPIO_Init( din.port, &g );
    return din;
}

Din din_event( char gpio, int pin, void (*handler)(int,int) )
{
    Din din = { .port    = gpio_enable( gpio )
              , .pin     = gpio_get_pin( pin )
              , .handler = handler
              };
    events[pin] = handler;
    e_port[pin] = din.port;
    GPIO_InitTypeDef g = { .Mode = GPIO_MODE_IT_RISING_FALLING
                         , .Pull = GPIO_PULLUP
                         , .Pin  = din.pin
                         };
    HAL_GPIO_Init( din.port, &g );

    int ex = din_get_irq( pin );
// FIXME
    HAL_NVIC_SetPriority( ex, 2, 0 ); // priority 2
    HAL_NVIC_EnableIRQ( ex );

    return din;
}

int din_get( Din* din )
{
    return HAL_GPIO_ReadPin( din->port, din->pin );
}

// brute force, don't check which are activated
void EXTI0_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler( gpio_get_pin(0) ); }
void EXTI1_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler( gpio_get_pin(1) ); }
void EXTI2_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler( gpio_get_pin(2) ); }
void EXTI3_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler( gpio_get_pin(3) ); }
void EXTI4_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler( gpio_get_pin(4) ); }
void EXTI9_5_IRQHandler( void ){
    for( int i=5; i<9; i++ ){
        HAL_GPIO_EXTI_IRQHandler( gpio_get_pin( i ) );
    }
}
void EXTI15_10_IRQHandler( void ){
    for( int i=10; i<15; i++ ){
        HAL_GPIO_EXTI_IRQHandler( gpio_get_pin( i ) );
    }
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
    int p = gpio_from_pin( GPIO_Pin );
    if( events[p] != NULL ){
        (events[p])(p, HAL_GPIO_ReadPin( e_port[p], GPIO_Pin ));
    }
}

static int din_get_irq( int pin )
{
    switch(pin){
        case 0: return EXTI0_IRQn;
        case 1: return EXTI1_IRQn;
        case 2: return EXTI2_TSC_IRQn;
        case 3: return EXTI3_IRQn;
        case 4: return EXTI4_IRQn;
        case 5: case 6: case 7: case 8: case 9: return EXTI9_5_IRQn;
        default: return EXTI15_10_IRQn;
    }
}
