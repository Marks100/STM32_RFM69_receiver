#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "misc.h"

#include "HAL_BRD.h"

false_true_et HAL_BRD_rtc_triggered_s;
false_true_et debug_mode;


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

	/* Configure the DEBUG selector pin, its important that this comes first */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* small delay to allow the button to settle */
	delay_us(500);

	debug_mode = HAL_BRD_read_debug_pin();

	#if( AUTO_DEBUG_MODE == 1 )
		debug_mode = ENABLE;
	#endif

	/* Setup the RF( RFM69 ) NCS Pin ( PB1 ) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Setup the RF ( RFM69 ) RESET Pin ( PB11 ) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	if( debug_mode == ENABLE )
	{
		/* Configure the GPIO_LED pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* Turn the led off straight away to save power */
		HAL_BRD_set_LED( OFF );

		/* Configure the wakeup ( or in debug mode interrupt ) pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* If we are in debug mode then we can configure the wakeup pin to have an ISR */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0 );

		EXTI_InitTypeDef EXTI_InitStruct;

		EXTI_InitStruct.EXTI_Line = EXTI_Line0 ;
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_Init(&EXTI_InitStruct);

		NVIC_InitTypeDef NVIC_InitStruct;

		/* Add IRQ vector to NVIC */
		NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
		/* Set priority */
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
		/* Set sub priority */
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
		/* Enable interrupt */
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		/* Add to NVIC */
		NVIC_Init(&NVIC_InitStruct);
	}

	/* Enable the "RFM69 packet received" interrupt on pin A1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1 );

	EXTI_InitTypeDef EXTI_InitStruct;

	EXTI_InitStruct.EXTI_Line = EXTI_Line1 ;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	HAL_BRD_RFM69_spi_slave_select( HIGH );
	HAL_BRD_RFM69_set_reset_Pin_state( LOW );

	HAL_BRD_rtc_triggered_s = TRUE;
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
low_high_et HAL_BRD_Read_Pin_state( GPIO_TypeDef * port, u16_t pin )
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
void HAL_BRD_Set_Pin_state(  GPIO_TypeDef * port, u16_t pin, low_high_et state )
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
void HAL_BRD_Toggle_Pin_state(  GPIO_TypeDef * port, u16_t pin )
{

    /* Firstly read the PIN state */
    if( ( port->ODR & pin ) == pin )
    {
        HAL_BRD_Set_Pin_state( port, pin, LOW );
    }
    else
    {
        HAL_BRD_Set_Pin_state( port, pin, HIGH );
    }
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
void HAL_BRD_Set_batt_monitor_state( disable_enable_et state )
{
	if( state == ENABLE )
	{
		//HAL_BRD_Set_Pin_state();
	}
	else
	{
		//HAL_BRD_Set_Pin_state();
	}
}


/**************************************************************************************************
EXTERNAL API's
***************************************************************************************************/




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
void HAL_BRD_RFM69_set_enable_Pin_state( low_high_et state )
{
	if( state == HIGH )
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
*   \brief         SETS the rf RST pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_RFM69_set_reset_Pin_state( low_high_et state )
{
	if( state == HIGH )
	{
		HAL_BRD_Set_Pin_state( GPIOB, GPIO_Pin_10, HIGH );
	}
	else
	{
		HAL_BRD_Set_Pin_state( GPIOB, GPIO_Pin_10, LOW );
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
void HAL_BRD_RFM69_spi_slave_select( low_high_et state )
{
	if( state == HIGH )
	{
		HAL_BRD_Set_Pin_state( GPIOB, GPIO_Pin_1, HIGH );
	}
	else
	{
		HAL_BRD_Set_Pin_state( GPIOB, GPIO_Pin_1, LOW );
	}
}




/*!
****************************************************************************************************
*
*   \brief         Toggles the led
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_Toggle_led( void )
{
    HAL_BRD_Toggle_Pin_state( GPIOC, GPIO_Pin_13 );
}


void HAL_BRD_set_LED( off_on_et state )
{
	low_high_et val;

	/* LED is inverse logic so flip the states */
	if( state == OFF )
	{
		val = HIGH;
	}
	else
	{
		val = LOW;
	}
	HAL_BRD_Set_Pin_state( GPIOC, GPIO_Pin_13, val);
}




/*!
****************************************************************************************************
*
*   \brief         Reads the state of the debug pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
disable_enable_et HAL_BRD_read_debug_pin( void )
{
	low_high_et state;
	disable_enable_et mode;

	state = HAL_BRD_Read_Pin_state(GPIOA, GPIO_Pin_4 );

	mode = (( state == HIGH ) ? ENABLE : DISABLE );

	return( mode );
}




/*!
****************************************************************************************************
*
*   \brief         Reads the state of the slider pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider )
{
    low_high_et state = LOW;

    switch ( slider )
    {
        case SLIDER_1:
            state = HAL_BRD_Read_Pin_state( GPIOB, GPIO_Pin_12 );
        break;

        case SLIDER_2:
            state = HAL_BRD_Read_Pin_state( GPIOB, GPIO_Pin_13 );
        break;

        case SLIDER_3:
            state = HAL_BRD_Read_Pin_state( GPIOB, GPIO_Pin_14 );
        break;

        case SLIDER_4:
            state = HAL_BRD_Read_Pin_state( GPIOB, GPIO_Pin_15 );
        break;

        default:
            break;
    }
    return ( state );
}


/*!
****************************************************************************************************
*
*   \brief         Toggles the heartbeat pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_Toggle_heartbeat_pin(  void )
{
    HAL_BRD_Toggle_Pin_state( GPIOB, GPIO_Pin_11 );
}



/*!
****************************************************************************************************
*
*   \brief         returns the  status of the RTC trigger
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
false_true_et HAL_BRD_get_rtc_trigger_status( void )
{
	return ( HAL_BRD_rtc_triggered_s );
}



void HAL_BRD_set_rtc_trigger_status( false_true_et state )
{
	HAL_BRD_rtc_triggered_s = state;
}




void EXTI0_IRQHandler(void)
{
	/* Make sure that interrupt flag is set */
	if ( EXTI_GetFlagStatus(EXTI_Line0) != RESET )
	{
		/* Now we keep track of the interrupt edge */
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line0);

		HAL_BRD_rtc_triggered_s = TRUE;
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

		RFM69_update_packet_received( TRUE );
	}
}







/*!
****************************************************************************************************
*
*   \brief         Interrupt Handler
*
*   \author        MS
*
*   \return        low_high_et
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


