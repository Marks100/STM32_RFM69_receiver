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
u32_t 			 NEOPIXEL_colour_tracker;

u16_t 			 NEOPIXEL_manual_pos_s;

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
	asm("nop");asm("nop");
	GPIOA->ODR &= ~GPIO_Pin_12;
}


STATIC void NEOPIXEL_zero_pulse_direct( void )
{
	/* This is soo time critical that instead of abstracting with function calls ill go direct to the hardware */
	GPIOA->ODR |= GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");

	GPIOA->ODR &= ~GPIO_Pin_12;
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
	NEOPIXEL_data_s.hl_state = NEOPIXEL_TEMPERATURE;
	NEOPIXEL_data_s.ll_state = NEOPIXEL_IDLE;
	NEOPIXEL_data_s.ticks = 0u;
	NEOPIXEL_data_s.led_num = 0u;
	NEOPIXEL_data_s.state_num = 0u;

	NEOPIXEL_tick_ctr_s = 0u;
	NEOPIXEL_colour_tracker = 0x00FF0000;

	NEOPIXEL_manual_pos_s = 1u;

	NEOPIXEL_clear_all_leds();
}



/* Sets a specif number of leds, all others are turned off */
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
	if( ( NEOPIXEL_tick_ctr_s % 500 ) == 0u )
	{
		switch( NEOPIXEL_data_s.hl_state )
		{
			case NEOPIXEL_ALL_FLASH:
				NEOPIXEL_set_hl_state( NEOPIXEL_CIRCLE_BUFFER );
				break;
			case NEOPIXEL_CIRCLE_BUFFER:
				NEOPIXEL_set_hl_state( NEOPIXEL_FLOW );
				break;
			case NEOPIXEL_FLOW:
				NEOPIXEL_set_hl_state( NEOPIXEL_CENTRE_SWEEP );
				break;
			case NEOPIXEL_CENTRE_SWEEP:
				NEOPIXEL_set_hl_state( NEOPIXEL_TEMPERATURE );
				break;
			case NEOPIXEL_TEMPERATURE:
				NEOPIXEL_set_hl_state( NEOPIXEL_TEMPERATURE );
				break;
			case NEOPIXEL_INDICATOR:
				NEOPIXEL_set_hl_state( NEOPIXEL_INDICATOR );
			break;

			case NEOPIXEL_MANUAL:
				NEOPIXEL_set_hl_state( NEOPIXEL_MANUAL );
				break;

			default:
				break;
		}
	}

	switch( NEOPIXEL_data_s.hl_state )
	{
		case NEOPIXEL_CENTRE_SWEEP:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_centre_sweep( &NEOPIXEL_data_s.ticks );
			break;

		case NEOPIXEL_ALL_FLASH:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_all_flash( &NEOPIXEL_data_s.ticks );
			break;

		case NEOPIXEL_CIRCLE_BUFFER:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_circle_buffer( &NEOPIXEL_data_s.ticks );
			break;

		case NEOPIXEL_FLOW:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_flow( &NEOPIXEL_data_s.ticks );
			break;

		case NEOPIXEL_TEMPERATURE:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_temperature_control( &NEOPIXEL_data_s.ticks );
			break;

		case NEOPIXEL_INDICATOR:
			NEOPIXEL_data_s.ll_state = NEOPIXEL_handle_audi_indicator( &NEOPIXEL_data_s.ticks );
			break;

		case NEOPIXEL_MANUAL:
			NEOPIXEL_data_s.ll_state =  NEOPIXEL_handle_manual( &NEOPIXEL_data_s.ticks );
			break;

		default:
			break;
	}

	if( NEOPIXEL_tick_ctr_s < U16_T_MAX )
	{
		NEOPIXEL_tick_ctr_s++;
	}
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_centre_sweep( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;

	if( ( (*ticks) % 4u ) == 0u )
	{
		if( NEOPIXEL_data_s.led_num == 0u )
		{
			NEOPIXEL_set_led( BV(NEOPIXEL_data_s.led_num), NEOPIXEL_RED );
		}
		else if( NEOPIXEL_data_s.led_num == ( NEOPIXEL_LED_MAX/2) )
		{
			NEOPIXEL_set_led( BV(NEOPIXEL_data_s.led_num), NEOPIXEL_GREEN );
		}
		else
		{
			NEOPIXEL_set_led( BV(NEOPIXEL_data_s.led_num) | BV(NEOPIXEL_LED_MAX - NEOPIXEL_data_s.led_num ), NEOPIXEL_RED );
		}
		NEOPIXEL_data_s.led_num ++;
	}

	if( NEOPIXEL_data_s.led_num >= NEOPIXEL_LED_MAX )
	{
		NEOPIXEL_data_s.led_num = 0u;
	}

	(*ticks)++;

	return( state );
}



NEOPIXEL_ll_state_et NEOPIXEL_handle_all_flash( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;

	if( (*ticks) == 0 )
	{
		NEOPIXEL_set_led( NEOPIXEL_ALL_LED, NEOPIXEL_RED );
	}

	(*ticks)++;

	return( state );
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_circle_buffer( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;

	if( ( (*ticks) % 4u ) == 0u )
	{
		NEOPIXEL_set_led( BV(NEOPIXEL_data_s.led_num), NEOPIXEL_BLUE );
		NEOPIXEL_data_s.led_num ++;
	}

	if( NEOPIXEL_data_s.led_num >= NEOPIXEL_LED_MAX )
	{
		NEOPIXEL_data_s.led_num = 0u;
	}

	(*ticks)++;

	return( state );
}

NEOPIXEL_ll_state_et NEOPIXEL_handle_flow( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;
	u8_t led_index = 0u;

	if( ( (*ticks) % 200u ) == 0u )
	{
		for( led_index = 0; led_index < NEOPIXEL_LED_MAX; led_index++ )
		{
			NEOPIXEL_drive_colour( NEOPIXEL_colour_tracker += 100 );
		}
	}

	if( NEOPIXEL_colour_tracker > NEOPIXEL_WHITE )
	{
		NEOPIXEL_colour_tracker = NEOPIXEL_WHITE;
	}

	(*ticks)++;

	return( state );
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_temperature_control( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;

	u32_t colour;

	switch ( NEOPIXEL_manual_pos_s )
	{
		case 0 ... 3:
			colour = NEOPIXEL_BLUE;
		break;

		case 4 ... 7:
			colour = NEOPIXEL_GREEN;
		break;

		case 8 ... 11:
			colour = NEOPIXEL_RED;
		break;

		default:
			colour = NEOPIXEL_RED;
			break;
	}

	NEOPIXEL_set_led( BV( NEOPIXEL_manual_pos_s ), colour );

	(*ticks)++;

	return( state );
}


NEOPIXEL_ll_state_et NEOPIXEL_handle_audi_indicator( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;
	u8_t led_index = 0u;
	u32_t led_to_set = 0u;

	u32_t colour = NEOPIXEL_ORANGE;

	if( ( (*ticks) % 1u ) == 0u )
	{
		switch ( NEOPIXEL_data_s.state_num )
		{
			case 0 ... 11:
			{
				for( led_index = 0u; led_index < NEOPIXEL_data_s.state_num+1; led_index++ )
				{
					led_to_set |= (BV(led_index));
				}
				NEOPIXEL_set_led( led_to_set, colour ); break;
			}
			break;

			case 18:
				NEOPIXEL_clear_all_leds();
				break;

			default:
				break;
		}
		NEOPIXEL_data_s.state_num++;
	}

	if( NEOPIXEL_data_s.state_num > 35 )
	{
		NEOPIXEL_data_s.state_num = 0u;

	}

	(*ticks)++;

	return( state );
}



NEOPIXEL_ll_state_et NEOPIXEL_handle_manual( u8_t* ticks )
{
	NEOPIXEL_ll_state_et state = NEOPIXEL_RUNNING;
	u8_t led_index = 0u;
	u32_t led_to_set = 0u;

	u32_t colour = NEOPIXEL_BLUE;

//	for( led_index = 0u; led_index <NEOPIXEL_manual_pos_s; led_index++ )
//	{
//		led_to_set |= (BV(led_index));
//	}
	led_to_set = (BV( NEOPIXEL_manual_pos_s - 1 ) );

	NEOPIXEL_set_led( led_to_set, colour );
}



void NEOPIXEL_set_hl_state( NEOPIXEL_hl_state_et state )
{
	NEOPIXEL_data_s.hl_state = state;
	NEOPIXEL_data_s.ll_state = NEOPIXEL_FINISHED;
	NEOPIXEL_data_s.ticks = 0u;
	NEOPIXEL_data_s.led_num = 0u;
	NEOPIXEL_data_s.state_num = 0u;
}


void NEOPIXEL_handle_rotary_input( ROTARY_scroll_type_et scroll )
{
	if( scroll == ROTARY_LEFT_SCROLL )
	{
		if( NEOPIXEL_manual_pos_s > 0 )
		{
			NEOPIXEL_manual_pos_s --;
		}
	}
	else if( scroll == ROTARY_RIGHT_SCROLL )
	{
		if( NEOPIXEL_manual_pos_s < NEOPIXEL_LED_MAX - 1u )
		{
			NEOPIXEL_manual_pos_s ++;
		}
	}
}


