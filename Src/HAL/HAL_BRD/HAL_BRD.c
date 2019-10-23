#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "misc.h"
#include "assert.h"
#include "core_cm3.h"

#include "HAL_config.h"
#include "HAL_BRD.h"
#include "STDC.h"
#include "HAL_TIM.h"
#include "MAIN.h"

EXTI_InitTypeDef EXTI_InitStruct;



/*!
****************************************************************************************************
*
*   \brief         Initialise the Pins,
*   			   lets just do gpio pins here and let other modules handle themselves
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_init( void )
{
	/* Disable the JTAG as this saves us some pins :) */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure the GPIOs */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the Onboard LED pin */
    GPIO_InitStructure.GPIO_Pin = ONBOARD_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(ONBOARD_LED_PORT, &GPIO_InitStructure);

    /* Configure the USER1 LED pin */
	GPIO_InitStructure.GPIO_Pin = USER_LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(USER_LED1_PORT, &GPIO_InitStructure);

    /* Configure the RELAY pin */
	GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(RELAY_PORT, &GPIO_InitStructure);


	/* Configure the NRF24 NCS pin */
	GPIO_InitStructure.GPIO_Pin = NRF24_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(NRF24_CS_PORT, &GPIO_InitStructure);

	/* Configure the NRF24 CE pin */
	GPIO_InitStructure.GPIO_Pin = NRF24_CE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(NRF24_CE_PORT, &GPIO_InitStructure);

	/* Configure the onboard switch */
	GPIO_InitStructure.GPIO_Pin = ONBOARD_BTN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ONBOARD_BTN_PORT, &GPIO_InitStructure);

    /* Turn the led off straight away to save power */
    HAL_BRD_CLR_ONBOARD_LED();
    HAL_BRD_CLR_RELAY();
}



/*!
****************************************************************************************************
*
*   \brief         Resets the board
*
*   \author        MS
*
*   \return        low_high_et
*
***************************************************************************************************/
void HAL_BRD_reset( void )
{
	NVIC_SystemReset();
}
	    




/*!
****************************************************************************************************
*
*   \brief         Reads Any PORT and any PIN
*
*   \author        MS
*
*   \return        low_high_et
*
***************************************************************************************************/
low_high_et HAL_BRD_read_pin_state( GPIO_TypeDef * port, u16_t pin )
{
	low_high_et returnType;

	if( ( port->IDR & pin ) == pin )
	{
		returnType = HIGH;
	}
	else
	{
		returnType = LOW;
	}

	return ( returnType );
}






/*!
****************************************************************************************************
*
*   \brief         SETS Any PIN on any PORT
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_set_pin_state(  GPIO_TypeDef * port, u16_t pin, low_high_et state )
{
	if( state == HIGH)
	{
		port->ODR |= pin;
	}
	else
	{
		port->ODR &= ~pin;
	}
}



/*!
****************************************************************************************************
*
*   \brief         Toggles Any PIN on any PORT
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_toggle_pin_state(  GPIO_TypeDef * port, u16_t pin )
{
    /* Firstly read the PIN state */
    if( ( port->ODR & pin ) == pin )
    {
        HAL_BRD_set_pin_state( port, pin, LOW );
    }
    else
    {
        HAL_BRD_set_pin_state( port, pin, HIGH );
    }
}



/*!
****************************************************************************************************
*
*   \brief         Reads the state of the onboard Button
*
*   \author        MS
*
*   \return        low_high_et
*
***************************************************************************************************/
low_high_et HAL_BRD_read_Onboard_btn_state( void )
{
	low_high_et returnType;

	returnType = HAL_BRD_read_pin_state( ONBOARD_BTN_PORT, ONBOARD_BTN_PIN );

	return ( returnType );
}





/*!
****************************************************************************************************
*
*   \brief         SETS the battery voltage enable pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_set_batt_monitor_state( disable_enable_et state )
{
	if( state == ENABLE_ )
	{
		//HAL_BRD_Set_Pin_state();
	}
	else
	{
		//HAL_BRD_Set_Pin_state();
	}
}




/*!
****************************************************************************************************
*
*   \brief         SETS the rf enable pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_NRF24_set_ce_pin_state( low_high_et state )
{
	if( state == HIGH )
	{
		HAL_BRD_set_pin_state( NRF24_CE_PORT, NRF24_CE_PIN, HIGH );
	}
	else
	{
		HAL_BRD_set_pin_state( NRF24_CE_PORT, NRF24_CE_PIN, LOW );
	}
}






/*!
****************************************************************************************************
*
*   \brief         SETS the SPI chip select pin for the RF module
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_NRF24_spi_slave_select( low_high_et state )
{
	if( state == HIGH )
	{
		HAL_BRD_set_pin_state( NRF24_CS_PORT, NRF24_CS_PIN, HIGH );
	}
	else
	{
		HAL_BRD_set_pin_state( NRF24_CS_PORT, NRF24_CS_PIN, LOW );
	}
}






void EXTI0_IRQHandler(void)
{
	/* Make sure that interrupt flag is set */
	if ( EXTI_GetFlagStatus(EXTI_Line0) != RESET )
	{
		/* Now we keep track of the interrupt edge */
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


void EXTI1_IRQHandler(void)
{
	/* Make sure that interrupt flag is set */
	if ( EXTI_GetFlagStatus(EXTI_Line1) != RESET )
	{
		/* Now we keep track of the interrupt edge */
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}


void EXTI9_5_IRQHandler(void)
{
	/* Make sure that interrupt flag is set */
	if ( EXTI_GetFlagStatus(EXTI_Line5) != RESET )
	{
		/* Now we keep track of the interrupt edge */
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}



/*!
****************************************************************************************************
*
*   \brief         Interrupt Handler
*
*   \author        MS
*
*
***************************************************************************************************/
/* Handle PB12 interrupt */
void EXTI15_10_IRQHandler(void)
{
	/* Make sure that interrupt flag is set */
	if ( EXTI_GetFlagStatus(EXTI_Line15) != RESET )
	{
	    /* Now we keep track of the interrupt edge */
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
