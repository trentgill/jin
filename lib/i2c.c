/*#include "i2c.h"

#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>

#include "gpio.h"

I2C_HandleTypeDef i2c_handle;

// public defns
I2C i2c_init( char port_scl, int pin_scl
            , char port_sda, int pin_sda
            , int address
            , void (*handler)(int)
            ){
    I2C ii;

    // FIXME: just requiring the known i2c pin pair
    if( !(port_scl == 'b' && pin_scl == 6
       && port_sda == 'b' && pin_sda == 7 )){
        printf("unknown i2c pin combo\n");
        return ii;
    }

    // FIXME totally unsure of this define. copied from crow&w/
    #define I2C_TIMING  0x50333090 // based on 400kHz @48MHz i2c clock
    i2c_handle.Instance              = I2C1; // hardcoded based on pins above
    i2c_handle.Init.Timing           = I2C_TIMING;
    i2c_handle.Init.OwnAddress1      = address << 1; // correct MSB justification
    i2c_handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    i2c_handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    i2c_handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    i2c_handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    if( HAL_I2C_Init( &i2c_handle ) ){
        printf("i2c failed to init\n");
    }

    //i2c_state.b_count  = 0;
    //i2c_state.b_head   = 0;
    //i2c_state.rxing    = 0;
    //i2c_state.tx_data  = NULL;
    //i2c_state.tx_bytes = 0;
    //for( uint8_t i=0; i<I2C_BUFFER_LEN; i++ ){
    //    for( uint8_t j=0; j<I2C_MAX_CMD_BYTES; j++ ){
    //        i2c_state.buffer[i][j] = 0;
    //    }
    //}
    //i2c_state.txing           = 0;
    //i2c_state.lead_rx_address = 0;
    //i2c_state.lead_rx_data    = NULL;
    //i2c_state.lead_rx_bytes   = 0;

    //BLOCK_IRQS(
        if( HAL_I2C_EnableListen_IT( &i2c_handle ) ){
            printf("I2c failed to enable listen\n");
        }
    //);

    ii.event = handler;
    return ii;
}

void i2c_deinit( I2C* ii )
{
    HAL_I2C_DeInit( &i2c_handle );
}

void i2c_send( I2C* ii, int address, int data )
{
    // TODO
}

void HAL_I2C_MspInit( I2C_HandleTypeDef* h )
{

// TODO but how to get the pin & ports in here?
// is this whole idea wrong? should it generate defines in a header or smthng?
    GPIO_InitTypeDef gpio;
    I2Cx_SCL_GPIO_CLK_ENABLE();
    I2Cx_SDA_GPIO_CLK_ENABLE();
    I2Cx_CLK_ENABLE();

    gpio.Pin       = I2Cx_SCL_PIN
                   | I2Cx_SDA_PIN;
    gpio.Mode      = GPIO_MODE_AF_OD;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init( I2Cx_SCL_GPIO_PORT, &gpio );

    // interrupt mode
    HAL_NVIC_SetPriority( I2Cx_ER_IRQn
                        , I2Cx_ER_Priority
                        , I2Cx_ER_SubPriority
                        );
    HAL_NVIC_EnableIRQ( I2Cx_ER_IRQn );

    HAL_NVIC_SetPriority( I2Cx_EV_IRQn
                        , I2Cx_EV_Priority
                        , I2Cx_EV_SubPriority
                        );
    HAL_NVIC_EnableIRQ( I2Cx_EV_IRQn );
}

void HAL_I2C_MspDeInit( I2C_HandleTypeDef* h )
{
	I2Cx_FORCE_RESET();
	I2Cx_RELEASE_RESET();

	HAL_NVIC_DisableIRQ( I2Cx_ER_IRQn );
	HAL_NVIC_DisableIRQ( I2Cx_EV_IRQn );

	HAL_GPIO_DeInit( I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN );
	HAL_GPIO_DeInit( I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN );
}


*/
