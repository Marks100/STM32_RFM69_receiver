/* STM32 specific includes */
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "STDC.h"
#include "NVM.h"
#include "HAL_BRD.h"
#include "MAIN.h"
#include "NEOPIXEL.h"


NEOPIXEL_data_st NEOPIXEL_data_s;
u16_t            NEOPIXEL_tick_ctr_s;

#pragma GCC push_options
#pragma GCC optimize ("O1")

STATIC void NEOPIXEL_one_pulse_direct( void )
{
	/* This is soo time critical that instead of abstracting with function calls ill go direct to the hardware */
	GPIOA->ODR |= GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");

	GPIOA->ODR &= ~GPIO_Pin_12;
}


STATIC void NEOPIXEL_zero_pulse_direct( void )
{
	/* This is soo time critical that instead of abstracting with function calls ill go direct to the hardware */
	GPIOA->ODR |= GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");

	GPIOA->ODR &= ~GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");
}


STATIC void NEOPIXEL_drive_colour( u32_t colour )
{
	u8_t bit = 0u;

	for ( bit = 0; bit < NEOPIXEL_CMD_BITS; bit++ )
	{
		if( ( colour & (BV(NEOPIXEL_CMD_BITS-1)>>bit) ) == (BV(NEOPIXEL_CMD_BITS-1)>>bit) )
		{
			NEOPIXEL_one_pulse_direct();
		}
		else
		{
			NEOPIXEL_zero_pulse_direct();
		}
	}
}

STATIC void NEOPIXEL_latch( void )
{
	u16_t bit = 0u;

	GPIOA->ODR &= ~GPIO_Pin_12;
	for ( bit = 0; bit < 500; bit++ )
	{
		asm("nop");
	}
}



void NEOPIXEL_int( void )
{
	NEOPIXEL_data_s.hl_state = NEOPIXEL_CENTRE_SWEEP;
	NEOPIXEL_data_s.ll_state = NEOPIXEL_IDLE;
	NEOPIXEL_data_s.stage = 0u;

	NEOPIXEL_tick_ctr_s = 0u;

	NEOPIXEL_clear_all_leds();
}


void NEOPIXEL_set_led( u32_t led_pos, u32_t colour )
{
	u8_t led_index = 0u;

	if( led_pos != NEOPIXEL_ALL_LED )
	{
		for( led_index = 0; led_index < NEOPIXEL_LED_MAX + 1; led_index++ )
		{
			if( ( led_pos & (BV(led_index ) ) ) != 0 )
			{
				NEOPIXEL_drive_colour( colour );
			}
			else
			{
				NEOPIXEL_drive_colour( NEOPIXEL_COLOUR_NONE );
			}
		}
	}
	else
	{
		NEOPIXEL_light_all_leds( colour );
	}
	NEOPIXEL_latch();
}


void NEOPIXEL_clear_all_leds( void )
{
	u8_t led_pos = 0;

	for ( led_pos = 0; led_pos < NEOPIXEL_LED_MAX; led_pos ++ )
	{
		NEOPIXEL_drive_colour( NEOPIXEL_COLOUR_NONE );
	}
	NEOPIXEL_latch();
}


void NEOPIXEL_light_all_leds( u32_t colour )
{
	u8_t led_pos = 0;

	for ( led_pos = 0; led_pos < NEOPIXEL_LED_MAX; led_pos ++ )
	{
		NEOPIXEL_drive_colour( colour );
	}
	NEOPIXEL_latch();
}

#pragma GCC pop_options



void NEOPIXEL_tick( void )
{
	switch( NEOPIXEL_tick_ctr_s )
	{
		case 500:
			NEOPIXEL_set_hl_state( NEOPIXEL_ALL_FLASH );
			break;
		case 1000:
			NEOPIXEL_set_hl_state( NEOPIXEL_CIRCLE_BUFFER );
			break;
		case 1500:
			NEOPIXEL_set_hl_state( NEOPIXEL_CENTRE_SWEEP );
			NEOPIXEL_tick_ctr_s = 0;
			break;
	}

	switch( NEOPIXEL_data_s.hl_state )
	{
		case NEOPIXEL_CENTRE_SWEEP:
				NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_centre_sweep( &NEOPIXEL_data_s.stage );
			break;

		case NEOPIXEL_ALL_FLASH:
				NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_all_flash( &NEOPIXEL_data_s.stage );
			break;

		case NEOPIXEL_CIRCLE_BUFFER:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_circle_buffer( &NEOPIXEL_data_s.stage );
			break;

		default:
			break;
	}

	if( NEOPIXEL_tick_ctr_s < U16_T_MAX )
	{
		NEOPIXEL_tick_ctr_s++;
	}
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_centre_sweep( u8_t* stage )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;
	u16_t temp_state = (*stage)++;

	switch( temp_state )
	{
		case 0:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED1), 0x0000FF00 );
			break;
		case 2:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED2) | BV(NEOPIXEL_LED12), 0x0000FF00 );
			break;
		case 4:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED3) | BV(NEOPIXEL_LED11), 0x0000FF00 );
			break;
		case 6:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED4) | BV(NEOPIXEL_LED10), 0x0000FF00 );
			break;
		case 8:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED5) | BV(NEOPIXEL_LED9), 0x0000FF00 );
			break;
		case 10:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED6) | BV(NEOPIXEL_LED8), 0x0000FF00 );
			break;
		case 12:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED7), 0x00FF0000 );
			break;
		case 14:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED6) | BV(NEOPIXEL_LED8), 0x0000FF00 );
			break;
		case 16:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED5) | BV(NEOPIXEL_LED9), 0x0000FF00 );
			break;
		case 18:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED4) | BV(NEOPIXEL_LED10), 0x0000FF00 );
			break;
		case 20:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED3) | BV(NEOPIXEL_LED11), 0x0000FF00 );
			break;
		case 22:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED2) | BV(NEOPIXEL_LED12), 0x0000FF00 );
			break;
		case 24:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED1), 0x0000FF00 );
			break;
		case 26:
			NEOPIXEL_clear_all_leds();
			(*stage) = 0u;
			break;

		default:
			break;
	}

	return( state );
}



NEOPIXEL_ll_state_et NEOPIXEL_handle_all_flash( u8_t* stage )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;

	switch( *stage )
	{
		case 0:
			NEOPIXEL_set_led( NEOPIXEL_ALL_LED, 0x0000FF00 );
			(*stage)++;
			break;

		default:
			break;
	}

	return( state );
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_circle_buffer( u8_t* stage )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;
	u16_t temp_state = (*stage)++;

	switch( temp_state )
	{
		case 0:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED1), 0x000000FF );
			break;
		case 2:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED2), 0x000000FF );
			break;
		case 4:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED3), 0x000000FF );
			break;
		case 6:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED4), 0x000000FF );
			break;
		case 8:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED5), 0x000000FF );
			break;
		case 10:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED6), 0x000000FF );
			break;
		case 12:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED7), 0x000000FF );
			break;
		case 14:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED6), 0x000000FF );
			break;
		case 16:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED8), 0x000000FF );
			break;
		case 18:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED9), 0x000000FF );
			break;
		case 20:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED10), 0x000000FF );
			break;
		case 22:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED11), 0x000000FF );
			break;
		case 24:
			NEOPIXEL_set_led( BV(NEOPIXEL_LED12), 0x000000FF );
			(*stage) = 0u;
			break;

		default:
			break;
	}

	return( state );
}



void NEOPIXEL_set_hl_state( NEOPIXEL_hl_state_et state )
{
	NEOPIXEL_data_s.hl_state = state;
	NEOPIXEL_data_s.ll_state = NEOPIXEL_FINISHED;
	NEOPIXEL_data_s.stage = 0u;
}


