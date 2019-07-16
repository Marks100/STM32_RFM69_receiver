#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "misc.h"
#include "assert.h"

#include "HAL_BRD.h"
#include "STDC.h"
#include "HAL_TIM.h"
#include "main.h"
#include "RFM69.h"
#include "NEOPIXEL.h"
#include "autoversion.h"

EXTI_InitTypeDef EXTI_InitStruct;
false_true_et HAL_BRD_rtc_triggered_s;
false_true_et debug_mode;

low_high_et HAL_BRD_rotary_clock;
low_high_et HAL_BRD_rotary_data;
low_high_et HAL_BRD_prev_rotary_clock;
low_high_et HAL_BRD_prev_rotary_data;
EXTITrigger_TypeDef trigger;



u16_t ROTARY_plus_cnt_s;
u16_t ROTARY_minus_cnt_s;


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

	/* Setup the RF ( RFM69 ) RESET Pin ( PB10 ) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Setup the Selector mode switches( PB6, PB7, PB8, PB9 ) */
	GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 );
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure the NRF24 NCS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the NRF24 CE pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the LED pins */
	GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure the NEO PIXEL pin */
	GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_12 );
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the "RFM69 packet received" interrupt on pin A1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the rotary clock and data pins */
	GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_8 | GPIO_Pin_11 );
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	NVIC_InitTypeDef NVIC_InitStruct;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11 );

	EXTI_InitStruct.EXTI_Line = EXTI_Line11 ;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = NVIC_PriorityGroup_0;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	HAL_BRD_RFM69_spi_slave_select( HIGH );
	HAL_BRD_RFM69_set_reset_pin_state( LOW );

	HAL_BRD_rtc_triggered_s = TRUE;

    /* Turn the led off straight away to save power */
    HAL_BRD_set_onboard_LED( OFF );



	HAL_BRD_rotary_clock = HIGH;
	HAL_BRD_rotary_data = HIGH;
	HAL_BRD_prev_rotary_clock = HIGH;
	HAL_BRD_prev_rotary_data = HIGH;
    trigger = EXTI_Trigger_Falling;





    ROTARY_plus_cnt_s = 0u;
    ROTARY_minus_cnt_s = 0u;

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







/**************************************************************************************************
EXTERNAL API's
***************************************************************************************************/
void HAL_BRD_set_LED_state( HAL_BRD_led_et led, off_on_et state )
{
	switch( led )
	{
		case LED_0:
			//HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_12, state );
			break;

		case LED_1:
			//HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_13, state );
			break;

		case LED_2:
			//HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_14, state );
			break;

		case LED_3:
			//HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_15, state );
			break;

		default:
			break;
	}
}


/*!
****************************************************************************************************
*
*   \brief         SETS the state of the NEO pixel pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
#pragma GCC push_options
#pragma GCC optimize ("O3")
void HAL_BRD_set_NEOpixel_state( low_high_et state )
{
	if( state == HIGH )
	{
		HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_12, state );
	}
	else
	{
		HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_12, state );
	}
}
#pragma GCC pop_options


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
void HAL_BRD_RFM69_set_enable_pin_state( low_high_et state )
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
		HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_10, HIGH );
	}
	else
	{
		HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_10, LOW );
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
void HAL_BRD_RFM69_set_reset_pin_state( low_high_et state )
{
	if( state == HIGH )
	{
		HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_10, HIGH );
	}
	else
	{
		HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_10, LOW );
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
		HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_1, HIGH );
	}
	else
	{
		HAL_BRD_set_pin_state( GPIOB, GPIO_Pin_1, LOW );
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
		HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_9, HIGH );
	}
	else
	{
		HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_9, LOW );
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
void HAL_BRD_toggle_led( void )
{
    HAL_BRD_toggle_pin_state( GPIOC, GPIO_Pin_13 );
}


void HAL_BRD_set_onboard_LED( off_on_et state )
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
	HAL_BRD_set_pin_state( GPIOC, GPIO_Pin_13, val);
}





/*!
****************************************************************************************************
*
*   \brief         Toggles the debug pin
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_toggle_debug_pin( void )
{
    HAL_BRD_toggle_pin_state( GPIOA, GPIO_Pin_8 );
}



void HAL_BRD_set_debug_pin( off_on_et state )
{
	HAL_BRD_set_pin_state( GPIOA, GPIO_Pin_8, state );
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

	state = HAL_BRD_read_pin_state(GPIOA, GPIO_Pin_4 );

	mode = (( state == HIGH ) ? ENABLE : DISABLE );

	return( mode );
}





/*!
****************************************************************************************************
*
*   \brief         Reads the state of the rotary clock pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
low_high_et HAL_BRD_read_rotary_clock_pin( void )
{
    low_high_et state = LOW;

    state = HAL_BRD_read_pin_state( GPIOA, GPIO_Pin_11 );

    return( state );
}

/*!
****************************************************************************************************
*
*   \brief         Reads the state of the rotary data pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
low_high_et HAL_BRD_read_rotary_data_pin( void )
{
    low_high_et state = LOW;

    state = HAL_BRD_read_pin_state( GPIOA, GPIO_Pin_8 );

    return( state );
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
            state = HAL_BRD_read_pin_state( GPIOB, GPIO_Pin_6 );
        break;

        case SLIDER_2:
            state = HAL_BRD_read_pin_state( GPIOB, GPIO_Pin_7 );
        break;

        case SLIDER_3:
            state = HAL_BRD_read_pin_state( GPIOB, GPIO_Pin_8 );
        break;

        case SLIDER_4:
            state = HAL_BRD_read_pin_state( GPIOB, GPIO_Pin_9 );
        break;

        default:
            break;
    }
    return ( state );
}



/*!
****************************************************************************************************
*
*   \brief         This gets called after the timer has debounced the LOGIC for the Rotary encoder
*
*   \author        MS
*
*   \return        low_high_et
*
***************************************************************************************************/
void HAL_BRD_debounce_completed( void )
{
	//HAL_BRD_toggle_led();

	/* Read the clock and data pin again */
	HAL_BRD_rotary_data = HAL_BRD_read_rotary_data_pin();
	HAL_BRD_rotary_clock = HAL_BRD_read_rotary_clock_pin();

	/* Check to make sure that the pin is the same state as it was in the ISR */
	if( HAL_BRD_prev_rotary_clock == HAL_BRD_rotary_clock )
	{
		/* Both the previous state and the current state are the same value and have been
		 * for at least 1ms so this is a valid pin transition
		 */
		if( trigger == EXTI_Trigger_Falling )
		{
			trigger = EXTI_Trigger_Rising;

			/* Check to make sure that the pin is the same state as it was in the ISR */
			if( HAL_BRD_rotary_data == HAL_BRD_prev_rotary_data )
			{
				ROTARY_evaluate_signals( HAL_BRD_rotary_clock, HAL_BRD_rotary_data );
			}
			else
			{
				/* do nothing */
				assert(1);
			}
		}
		else
		{
			trigger = EXTI_Trigger_Falling;
		}
	}
	else
	{
		/* The pins are not the same state for at least 1ms so this
		 * is probably a glitch that needs debounced ta fuck */
	}

	EXTI_InitStruct.EXTI_Line = EXTI_Line11 ;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = trigger;
	EXTI_Init(&EXTI_InitStruct);

	//HAL_BRD_toggle_led();

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
void HAL_BRD_toggle_heartbeat_pin(  void )
{
    HAL_BRD_toggle_pin_state( GPIOB, GPIO_Pin_11 );
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


void HAL_BRD_set_heater_state( off_on_et state )
{
	HAL_BRD_set_LED_state( LED_3, state );
}


void HAL_BRD_set_cooler_state( off_on_et state )
{
	HAL_BRD_set_LED_state( LED_3, state );
}


void HAL_BRD_set_ROTARY_interrupt_state( disable_enable_et state )
{
	if( state == ENABLE_ )
	{
		EXTI_InitStruct.EXTI_Line = EXTI_Line11 ;
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
		EXTI_InitStruct.EXTI_Trigger = trigger;
		EXTI_Init(&EXTI_InitStruct);
	}
	else
	{
		EXTI_InitStruct.EXTI_Line = EXTI_Line11 ;
		EXTI_InitStruct.EXTI_LineCmd = DISABLE;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_Init(&EXTI_InitStruct);
	}
}


/*!
****************************************************************************************************
*
*   \brief         returns the SW version number
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_get_SW_version_num( u8_t *version_num_p )
{
	/* Reference the included autoversion.h file so that we can grab the SW version number */
	char version_num_array[SW_VERSION_NUM_SIZE];

	version_num_array[0] = STM32_RF_HUB_VERSION_MAJOR;
	version_num_array[1] = STM32_RF_HUB_VERSION_PATCH;
	version_num_array[2] = STM32_RF_HUB_VERSION_VERIFICATION;

	/* Now copy the local version array back to the pointer */
	STDC_memcpy( version_num_p, version_num_array, sizeof( version_num_array ) );
}



/*!
****************************************************************************************************
*
*   \brief         returns the HAL_BRD_NRF24_spi_slave_selectW version number
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void HAL_BRD_get_HW_version_num( u8_t *version_num_p )
{
	*version_num_p = 0u;
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
	HAL_BRD_toggle_led();

	/* Make sure that interrupt flag is set */
	if ( EXTI_GetFlagStatus(EXTI_Line15) != RESET )
	{
	    /* Now we keep track of the interrupt edge */
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line15);
	}

	if ( EXTI_GetFlagStatus(EXTI_Line11) != RESET )
	{
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line11);

		/* grab the currect state of the data pin */
		HAL_BRD_prev_rotary_data = HAL_BRD_read_rotary_data_pin();

		/* Now we keep track of the interrupt edge */
		if( trigger == EXTI_Trigger_Falling )
		{
			HAL_BRD_prev_rotary_clock = LOW;
		}
		else
		{
			HAL_BRD_prev_rotary_clock = HIGH;
		}

		EXTI_InitStruct.EXTI_Line = EXTI_Line11 ;
		EXTI_InitStruct.EXTI_LineCmd = DISABLE;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_Init(&EXTI_InitStruct);

		/* Start a timer to generate a callback in xms to debounce the LOGIC */
		HAL_TIM2_start();
		HAL_BRD_toggle_led();
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



















void ROTARY_evaluate_signals( low_high_et clock, low_high_et data )
{
 	if( ( clock == LOW ) && ( data == HIGH ) )
	{
 		ROTARY_minus_cnt_s ++;
	}
	else if( ( clock == LOW ) && ( data == LOW ) )
	{
		ROTARY_plus_cnt_s ++;
	}
}


/*!
****************************************************************************************************
*
*   \brief         This is USER configurable and should call out to all modules that are relevant
*                  to inform them of the current
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void ROTARY_tick( void )
{
	u16_t i = 0u;
	u16_t left = ROTARY_minus_cnt_s;
	u16_t right = ROTARY_plus_cnt_s;

	/* Reset the current counts */
	ROTARY_minus_cnt_s = 0u;
	ROTARY_plus_cnt_s = 0u;

	/* Handle all the left scrolls */
	for( i = 0u; i < left; i++ )
	{
		NEOPIXEL_handle_rotary_input( ROTARY_LEFT_SCROLL );
	}

	/* Handle all the left scrolls */
	for( i = 0u; i < right; i++ )
	{
		NEOPIXEL_handle_rotary_input( ROTARY_RIGHT_SCROLL );
	}
}
