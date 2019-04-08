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
STATIC u8_t                    MODE_MGR_selector_switch_state_s;
STATIC u8_t                    MODE_MGR_debounce_ctr_s;
extern NVM_info_st NVM_info_s;



void MODE_MGR_init( void )
{
	MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
	MODE_MGR_mode_s = MODE_MGR_MODE_NORMAL;
	MODE_MGR_system_init_s = FALSE;
	MODE_MGR_selector_switch_state_s = 0u;
	MODE_MGR_debounce_ctr_s = 0xFF;
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
    HEATING_tick();

    switch( MODE_MGR_tick_timer_msecs_s )
    {
        case 20u:
        	break;

        case 40u:
            //RFM69_tick();
        	break;

        case 60u:
            //MODE_MGR_analyse_switches();
        	break;

        case 80u:
        	NVM_tick();
        	//RFM69_tick();
        	break;

        case 100u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 120u:
            //RFM69_tick();
        	break;

        case 140u:
        	break;

        case 160u:
            //RFM69_tick();
            //MODE_MGR_analyse_switches();
        	break;

        case 180u:
        	NVM_tick();
        	break;

        case 200u:
        	MAIN_WATCHDOG_kick();
            //RFM69_tick();
            RF_MGR_tick();
        	break;

        case 220u://RFM69_tick
        	break;

        case 240u:
            //RFM69_tick();
        	break;

        case 260u:
            //MODE_MGR_analyse_switches();
        	break;

        case 280u:
        	NVM_tick();
        	//RFM69_tick();
        	break;

        case 300u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 320u:
            //RFM69_tick();
        	break;

        case 340u:
        	break;

        case 360u:
            //RFM69_tick();
            //MODE_MGR_analyse_switches();
        	break;

        case 380u:
        	NVM_tick();
        	break;

        case 400u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
            //RFM69_tick();
        	break;

        case 420u:
        	break;

        case 440u:
            //RFM69_tick();
        	break;

        case 460u:
            //MODE_MGR_analyse_switches();
        	break;

        case 480u:
        	NVM_tick();
        	//RFM69_tick();
        	break;

        case 500u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 520u:
            //RFM69_tick();
        	break;

        case 540u:
        	break;

        case 560u:
            //RFM69_tick();
            //MODE_MGR_analyse_switches();
        	break;

        case 580u:
        	NVM_tick();
        	break;

        case 600u:
        	MAIN_WATCHDOG_kick();
            //RFM69_tick();
            RF_MGR_tick();
        	break;

        case 620u:
        	break;

        case 640u:
            //RFM69_tick();
        	break;

        case 660u:
           // MODE_MGR_analyse_switches();
        	break;

        case 680u:
        	NVM_tick();
        	//RFM69_tick();
        	break;

        case 700u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 720u:
            //RFM69_tick();
        	break;

        case 740u:
        	break;

        case 760u:
            //RFM69_tick();
            //MODE_MGR_analyse_switches();
        	break;

        case 780u:
        	NVM_tick();
        	break;

        case 800u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
            //RFM69_tick();
        	break;

        case 820u:
        	break;

        case 840u:
            //RFM69_tick();
        	break;

        case 860u:
            //MODE_MGR_analyse_switches();
        	break;

        case 880u:
        	NVM_tick();
        	//RFM69_tick();
        	break;

        case 900u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
        	break;

        case 920u:
            //RFM69_tick();
        	break;

        case 940u:
        	break;

        case 960u:
            //RFM69_tick();
            //MODE_MGR_analyse_switches();
        	break;

        case 980u:
        	NVM_tick();
        	break;

        case 1000u:
        	MAIN_WATCHDOG_kick();
            RF_MGR_tick();
            //RFM69_tick();

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



void MODE_MGR_analyse_switches( void )
{
    u8_t i;
    u8_t slider_mask = 0;

    /* Loop through all the positions and analyse them */
    for( i = SLIDER_1; i < SLIDER_MAX; i ++ )
    {
        slider_mask |= ( HAL_BRD_read_selector_switch_pin( (HAL_BRD_switch_slider_et)i ) << i );
    }

    if( ( MODE_MGR_selector_switch_state_s & SELECTOR_MODE_BIT_MASK ) != ( slider_mask & SELECTOR_MODE_BIT_MASK ) )
    {
        /* selector switch positions have changed */
        MODE_MGR_selector_switch_state_s = slider_mask;

        MODE_MGR_debounce_ctr_s = 0u;
    }

    /* Rudementary debounce mechanism */
    if( MODE_MGR_debounce_ctr_s != 0xFF )
    {
        if( MODE_MGR_debounce_ctr_s >= 4 )
        {
            MODE_MGR_debounce_ctr_s = 0xFF;

            MODE_MGR_action_selector_switch_changes( MODE_MGR_selector_switch_state_s );
        }
        else
        {
        	MODE_MGR_debounce_ctr_s ++;
        }
    }
}




void MODE_MGR_action_selector_switch_changes( u8_t MODE_MGR_selector_switch_state_s )
{
    switch( MODE_MGR_selector_switch_state_s )
    {
        case COMMAND_1:
            HAL_BRD_toggle_debug_pin();
            break;

        default:
            HAL_BRD_toggle_debug_pin();
            break;
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


