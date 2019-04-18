/* STM32 specific includes */
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "MODE_MGR.h"
#include "STDC.h"
#include "NVM.h"
#include "HAL_ADC.h"
#include "HAL_BRD.h"
#include "HAL_TIM.h"
#include "MAIN.h"
#include "HEATING.h"
#include "CLI.h"
#include "RFM69.h"
#include "HAL_UART.h"


STATIC false_true_et 		   MODE_MGR_system_init_s;
STATIC u32_t 			       MODE_MGR_tick_timer_msecs_s;
STATIC MODE_MGR_mode_et        MODE_MGR_mode_s;
STATIC u8_t                    MODE_MGR_debounce_ctr_s;

STATIC MODE_MGR_user_input_st  MODE_MGR_user_input_s[MODE_MGR_NUM_SLIDER_INPUTS];
extern NVM_info_st NVM_info_s;




#pragma GCC push_options
#pragma GCC optimize ("O0")

void one_pulse_direct( void )
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

	GPIOA->ODR &= ~GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
}


void zero_pulse_direct( void )
{
	/* This is soo time critical that instead of abstracting with function calls ill go direct to the hardware */
	GPIOA->ODR |= GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");

	GPIOA->ODR &= ~GPIO_Pin_12;
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
}



void NEOpixel_set_led( u32_t led_pos, u32_t colour )
{
	u8_t bit = 0u;
	u8_t led_index = 0u;

	for( led_index = 0; led_index < NEOPIXEL_LED_MAX; led_index++ )
	{
		if( ( led_pos & (BV(NEOPIXEL_LED_MAX - led_index - 1) ) ) != 0 )
		{
			for ( bit = 0; bit < 24; bit++ )
			{
				if( ( colour & (0x800000>>bit) ) == 0x800000>>bit )
				{
					one_pulse_direct();
				}
				else
				{
					zero_pulse_direct();
				}
			}
		}
		else
		{
			for ( bit = 0; bit < 24; bit++ )
			{
				zero_pulse_direct();
			}
		}
	}
}

void NEOpixel_latch( void )
{
	u16_t bit = 0u;

	GPIOA->ODR &= ~GPIO_Pin_12;
	for ( bit = 0; bit < 500; bit++ )
	{
		asm("nop");
	}
}

void NEOpixel_clear_all_leds( void )
{
	u8_t led_pos = 0;
	u8_t bit = 0u;

	for ( led_pos = 0; led_pos < NEOPIXEL_LED_MAX; led_pos ++ )
	{
		for ( bit = 0; bit < 24; bit++ )
		{
			zero_pulse_direct();
		}
	}
	NEOpixel_latch();
}

void NEOpixel_light_all_leds( u32_t colour )
{
	u8_t led_pos = 0;
	u8_t bit = 0u;

	for ( led_pos = 0; led_pos < NEOPIXEL_LED_MAX; led_pos ++ )
	{
		for ( bit = 0; bit < 24; bit++ )
		{
			if( ( colour & (0x800000>>bit) ) == 0x800000>>bit )
			{
				one_pulse_direct();
			}
			else
			{
				zero_pulse_direct();
			}
		}
	}
	NEOpixel_latch();
}

#pragma GCC pop_options


void MODE_MGR_init( void )
{
	u8_t i = 0u;

	MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
	MODE_MGR_mode_s = MODE_MGR_MODE_NORMAL;
	MODE_MGR_system_init_s = FALSE;
	MODE_MGR_debounce_ctr_s = 0xFF;

	STDC_memset( &MODE_MGR_user_input_s, 0x00, sizeof( MODE_MGR_user_input_s ) );

	for( i = 0u; i < MODE_MGR_NUM_SLIDER_INPUTS; i++ )
	{
		/* Need to set the high level state initial on reset, so just read the current state of the selector switches */
		MODE_MGR_user_input_s[i].slider_state_hl = HAL_BRD_read_selector_switch_pin( (HAL_BRD_switch_slider_et)i );
	}
}



void MODE_MGR_20ms_tick( void )
{
	switch( MODE_MGR_mode_s )
	{
		case MODE_MGR_MODE_NORMAL:
			MODE_MGR_action_schedule_normal();
		break;

		case MODE_MGR_MODE_TEST:
		break;

		case MODE_MGR_SETTINGS_MODE:
			MODE_MGR_action_schedule_normal();
			break;

		default:
			break;
	}
}


/*!
****************************************************************************************************
*
*   \brief          < Brief description of function >
*
*   \author
*
*   \return         none
*
***************************************************************************************************/
void MODE_MGR_action_schedule_normal( void )
{
    NRF24_tick();
    CLI_message_handler();
    MODE_MGR_check_user_input();
    u8_t i = 0u;







    switch( MODE_MGR_tick_timer_msecs_s )
    {
        case 20u:
        	NEOpixel_clear_all_leds();
        	break;

        case 40u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED1), 0x0000FF00 );
            HEATING_tick();
        	break;

        case 60u:
        	break;

        case 80u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED2) | BV(NEOPIXEL_LED12) , 0x0000FF00 );
        	NVM_tick();
        	break;

        case 100u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 120u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED3) | BV(NEOPIXEL_LED11),0x0000FF00 );
        	break;

        case 140u:
            HEATING_tick();
        	break;

        case 160u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED4) | BV(NEOPIXEL_LED10),0x0000FF00 );
        	break;

        case 180u:
        	NVM_tick();
        	break;

        case 200u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
            NEOpixel_set_led( BV(NEOPIXEL_LED5) | BV(NEOPIXEL_LED9),0x0000FF00 );
        	break;

        case 220u:
        	break;

        case 240u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED7) | BV(NEOPIXEL_LED8),0x0000FF00 );
            HEATING_tick();
        	break;

        case 260u:
        	break;

        case 280u:
        	NVM_tick();
        	NEOpixel_set_led( BV(NEOPIXEL_LED7),0x00FF0000 );
        	break;

        case 300u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 320u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED7) | BV(NEOPIXEL_LED8),0x0000FF00 );
        	break;

        case 340u:
            HEATING_tick();
        	break;

        case 360u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED5) | BV(NEOPIXEL_LED9),0x0000FF00 );
        	break;

        case 380u:
        	NVM_tick();
        	break;

        case 400u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
            NEOpixel_set_led( BV(NEOPIXEL_LED4) | BV(NEOPIXEL_LED10),0x0000FF00 );
        	break;

        case 420u:
        	break;

        case 440u:
            HEATING_tick();
            NEOpixel_set_led( BV(NEOPIXEL_LED3) | BV(NEOPIXEL_LED11),0x0000FF00 );
        	break;

        case 460u:
        	break;

        case 480u:
        	NVM_tick();
        	NEOpixel_set_led( BV(NEOPIXEL_LED2) | BV(NEOPIXEL_LED10),0x0000FF00 );
        	break;

        case 500u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 520u:
        	NEOpixel_set_led( BV(NEOPIXEL_LED1),0x00FF0000 );
        	break;

        case 540u:
            HEATING_tick();
        	break;

        case 560u:
        	break;

        case 580u:
        	NVM_tick();
        	break;

        case 600u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 620u:
        	break;

        case 640u:
            HEATING_tick();
        	break;

        case 660u:
        	break;

        case 680u:
        	NVM_tick();
        	break;

        case 700u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 720u:
        	break;

        case 740u:
            HEATING_tick();
        	break;

        case 760u:
        	break;

        case 780u:
        	NVM_tick();
        	break;

        case 800u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 820u:
        	break;

        case 840u:
            HEATING_tick();
        	break;

        case 860u:
        	break;

        case 880u:
        	NVM_tick();
        	break;

        case 900u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 920u:
        	break;

        case 940u:
            HEATING_tick();
        	break;

        case 960u:
        	break;

        case 980u:
        	NVM_tick();
        	break;

        case 1000u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();

            /* keep track of time in secs */
            HAL_TIM_increment_secs();
        	break;

			default:
			break;
    }

	if( MODE_MGR_tick_timer_msecs_s >= MODE_MGR_MAX_TICK_CYCLE_VALUE )
	{
		MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
	}
	else
	{
		MODE_MGR_tick_timer_msecs_s += MODE_MGR_TICK_RATE_MSECS;
	}
}




MODE_MGR_mode_et MODE_MGR_get_mode( void )
{
	return MODE_MGR_mode_s;
}




void MODE_MGR_change_mode( void )
{
	false_true_et reset_value = FALSE;
	(void)reset_value;

	switch( MODE_MGR_mode_s )
	{
		case MODE_MGR_MODE_NORMAL:
			break;

		case MODE_MGR_SETTINGS_MODE:
		    break;

        default:
            break;
	}
}




void MODE_MGR_action_selector_switch_changes( HAL_BRD_switch_slider_et slider, low_high_et state )
{
	switch ( slider )
	{
		case SLIDER_1:
			if( HEATING_get_state() == ENABLE_ )
			{
				HEATING_set_state( DISABLE_ );
			}
			else
			{
				HEATING_set_state( ENABLE_ );
			}
			break;


		case SLIDER_2:
			/* This switch has changed state, we dont care what the actual state is ( high or low ) 0 as long as we detect a change */
			if( HEATING_get_mode() == HEATING_HEAT_MODE )
			{
				HEATING_set_mode( HEATING_COOL_MODE );
			}
			else
			{
				HEATING_set_mode( HEATING_HEAT_MODE );
			}
			break;

		default:
			break;
	}
}




void MODE_MGR_check_user_input( void )
{
	u8_t i = 0u;

	/* First read the low level state of the pins */
	for( i = 0u; i < MODE_MGR_NUM_SLIDER_INPUTS; i++ )
	{
		MODE_MGR_user_input_s[i].slider_state_ll = HAL_BRD_read_selector_switch_pin( (HAL_BRD_switch_slider_et)i );

		/* If the low level state is != the high level state then start the debounce */
		if( MODE_MGR_user_input_s[i].slider_state_ll != MODE_MGR_user_input_s[i].slider_state_hl )
		{
			if( MODE_MGR_user_input_s[i].slider_debounce_time < U8_T_MAX )
			{
				MODE_MGR_user_input_s[i].slider_debounce_time++;
			}
		}
		else
		{
			MODE_MGR_user_input_s[i].slider_debounce_time = 0u;
		}
	}

	/* Now check if the debounce times have been elapsed */
	for( i = 0u; i < MODE_MGR_NUM_SLIDER_INPUTS; i++ )
	{
		if( MODE_MGR_user_input_s[i].slider_debounce_time >= MODE_MGR_SLIDER_DB_TICKS )
		{
			/* Toggle the high level state */
			MODE_MGR_user_input_s[i].slider_state_hl ^= 1u;
			MODE_MGR_user_input_s[i].slider_debounce_time = 0u;

			MODE_MGR_action_selector_switch_changes( (HAL_BRD_switch_slider_et)i, MODE_MGR_user_input_s[i].slider_state_hl );
		}
	}
}




/*!
****************************************************************************************************
*
*   \brief          Requests a SPOT weld so that on the next MODE_MGR tick it gets carried out
*
*   \author
*
*   \return         none
*
***************************************************************************************************/
void MODE_MGR_set_system_init_status( false_true_et state )
{
	MODE_MGR_system_init_s = state;
}











