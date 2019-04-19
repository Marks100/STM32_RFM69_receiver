/* STM32 specific includes */
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "STDC.h"
#include "NVM.h"
#include "HAL_BRD.h"
#include "MAIN.h"
#include "NEOPIXEL.h"


NEOPIXEL_data_st NEOPIXEL_data_s;

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

	NEOPIXEL_clear_all_leds();
}


void NEOPIXEL_set_led( u32_t led_pos, u32_t colour )
{
	u8_t led_index = 0u;

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
	switch( NEOPIXEL_data_s.hl_state )
	{
		case NEOPIXEL_CENTRE_SWEEP:
				NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_centre_sweep( NEOPIXEL_data_s.stage );
			break;

		default:
			break;
	}
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_centre_sweep( u8_t* stage )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;

	switch( *stage )
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			state = NEOPIXEL_FINISHED;
			break;

		default:
			break;
	}

	(*stage)++;

	return( state );
}



