/* STM32 specific includes */
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "MODE_MGR.h"
#include "STDC.h"
#include "NVM.h"
#include "HAL_BRD.h"
#include "HAL_TIM.h"
#include "WDG.h"
#include "BTN_MGR.h"
#include "HAL_UART.h"




STATIC false_true_et 		   MODE_MGR_system_init_s;
STATIC u32_t 			       MODE_MGR_tick_timer_msecs_s;
STATIC MODE_MGR_mode_et        MODE_MGR_mode_s;
STATIC u8_t                    MODE_MGR_debounce_ctr_s;

STATIC MODE_MGR_user_input_st  MODE_MGR_user_input_s[MODE_MGR_NUM_SLIDER_INPUTS];
extern NVM_info_st NVM_info_s;



void MODE_MGR_init( void )
{
	u8_t i = 0u;

	MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
	MODE_MGR_mode_s = MODE_MGR_MODE_STARTUP;
	MODE_MGR_system_init_s = FALSE;
	MODE_MGR_debounce_ctr_s = 0xFF;

	STDC_memset( &MODE_MGR_user_input_s, 0x00, sizeof( MODE_MGR_user_input_s ) );

	for( i = 0u; i < MODE_MGR_NUM_SLIDER_INPUTS; i++ )
	{
		/* Need to set the high level state initial on reset, so just read the current state of the selector switches */
		//MODE_MGR_user_input_s[i].slider_state_hl = HAL_BRD_read_selector_switch_pin( (HAL_BRD_switch_slider_et)i );
	}
}



void MODE_MGR_tick( void )
{
	WDG_kick();

	switch( MODE_MGR_mode_s )
	{
		case MODE_MGR_MODE_STARTUP:
			if( LOW == HAL_BRD_read_Onboard_btn_state() )
			{
				MODE_MGR_change_mode( MODE_MGR_MODE_WAIT );
				HAL_BRD_SET_ONBOARD_LED();
			}
			else
			{
				MODE_MGR_change_mode( MODE_MGR_MODE_NORMAL );
			}
		break;

		case MODE_MGR_MODE_WAIT:
			if( HIGH == HAL_BRD_read_Onboard_btn_state() )
			{
				MODE_MGR_change_mode( MODE_MGR_MODE_TEST );
			}

			HAL_BRD_SET_ONBOARD_LED();
		break;

		case MODE_MGR_MODE_NORMAL:
			MODE_MGR_action_schedule_normal();
		break;

		case MODE_MGR_MODE_TEST:
			MODE_MGR_action_schedule_test();

			if( LOW == HAL_BRD_read_Onboard_btn_state() )
			{
				MODE_MGR_change_mode( MODE_MGR_MODE_NORMAL );
			}
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
	HAL_BRD_TOGGLE_ONBOARD_LED();
	NRF24_tick();
	BTN_MGR_tick();
	RF_MGR_tick();

    switch( MODE_MGR_tick_timer_msecs_s )
    {
        case 50u:
        	break;

        case 100u:
        	break;

        case 150u:
        	break;

        case 200u:
			NVM_tick();
        	break;

        case 250u:
        	break;

        case 300u:
        	break;

        case 350u:
        	break;

        case 400u:
			NVM_tick();
        	break;

        case 450u:
        	break;

        case 500u:
        	break;

        case 550u:
        	break;

        case 600u:
			NVM_tick();
        	break;

        case 650u:
        	break;

        case 700u:
        	break;

        case 750u:
        	break;

        case 800u:
			NVM_tick();
        	break;

        case 850u:
        	break;

        case 900u:
        	break;

        case 950u:
        	break;

        case 1000u:
			NVM_tick();

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
void MODE_MGR_action_schedule_test( void )
{
	if( MODE_MGR_tick_timer_msecs_s >= MODE_MGR_MAX_TICK_CYCLE_VALUE )
	{
		MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;

		HAL_BRD_TOGGLE_ONBOARD_LED();
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




void MODE_MGR_change_mode( MODE_MGR_mode_et mode )
{
	MODE_MGR_mode_s = mode;
}




void MODE_MGR_action_selector_switch_changes( HAL_BRD_switch_slider_et slider, low_high_et state )
{
	switch ( slider )
	{
		case SLIDER_1:
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











