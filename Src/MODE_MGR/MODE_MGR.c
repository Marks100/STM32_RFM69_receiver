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

STATIC MODE_MGR_user_input_st  MODE_MGR_user_input_s[MODE_MGR_NUM_SLIDER_IMPUTS];
extern NVM_info_st NVM_info_s;



void MODE_MGR_init( void )
{
	MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
	MODE_MGR_mode_s = MODE_MGR_MODE_NORMAL;
	MODE_MGR_system_init_s = FALSE;
	MODE_MGR_debounce_ctr_s = 0xFF;

	STDC_memset( &MODE_MGR_user_input_s, 0x00, sizeof( MODE_MGR_user_input_s ) );
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

    switch( MODE_MGR_tick_timer_msecs_s )
    {
        case 20u:
        	break;

        case 40u:
            HEATING_tick();
        	break;

        case 60u:
        	break;

        case 80u:
        	NVM_tick();
        	break;

        case 100u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 120u:
        	break;

        case 140u:
            HEATING_tick();
        	break;

        case 160u:
        	break;

        case 180u:
        	NVM_tick();
        	break;

        case 200u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 220u:
        	break;

        case 240u:
            HEATING_tick();
        	break;

        case 260u:
        	break;

        case 280u:
        	NVM_tick();
        	break;

        case 300u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 320u:
        	break;

        case 340u:
            HEATING_tick();
        	break;

        case 360u:
        	break;

        case 380u:
        	NVM_tick();
        	break;

        case 400u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 420u:
        	break;

        case 440u:
            HEATING_tick();
        	break;

        case 460u:
        	break;

        case 480u:
        	NVM_tick();
        	break;

        case 500u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 520u:
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
			if( state == HIGH )
			{
				HEATING_set_mode( HEATING_HEAT_MODE );
			}
			else
			{
				HEATING_set_mode( HEATING_COOL_MODE );
			}
			break;

		case SLIDER_2:
			break;

		default:
			break;
	}
}




void MODE_MGR_check_user_input( void )
{
	u8_t i = 0u;

	/* First read the low level state eof the pins */
	for( i = 0u; i < MODE_MGR_NUM_SLIDER_IMPUTS; i++ )
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
	for( i = 0u; i < MODE_MGR_NUM_SLIDER_IMPUTS; i++ )
	{
		if( MODE_MGR_user_input_s[i].slider_debounce_time >= MODE_MGR_SLIDER_DB_TICKS )
		{
			/* Toggle the high level state */
			MODE_MGR_user_input_s[i].slider_state_hl ^= 1;
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


