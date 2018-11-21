/* STM32 specific includes */
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "MODE_MGR.h"
#include "STDC.h"
#include "NVM.h"
#include "HAL_ADC.h"
#include "HAL_BRD.h"
#include "HAL_TIM.h"


STATIC false_true_et 		   MODE_MGR_system_init;
STATIC u32_t 			       MODE_MGR_tick_timer_msecs_s;
STATIC MODE_MGR_mode_et        MODE_MGR_mode_s;

extern NVM_info_st NVM_info_s;




void MODE_MGR_init( void )
{
	MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
	MODE_MGR_mode_s = MODE_MGR_MODE_NORMAL;
	MODE_MGR_system_init = FALSE;
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
    switch( MODE_MGR_tick_timer_msecs_s )
    {
        case 20u:
            SERIAL_msg_handler();
        	break;

        case 40u:
            RFM69_receive_frame();
        	break;

        case 60u:
        	break;

        case 80u:
        	NVM_tick();
        	RFM69_receive_frame();
        	break;

        case 100u:
        	break;

        case 120u:
            SERIAL_msg_handler();
            RFM69_receive_frame();
        	break;

        case 140u:
        	break;

        case 160u:
            RFM69_receive_frame();
        	break;

        case 180u:
        	NVM_tick();
        	break;

        case 200u:
            RFM69_receive_frame();
        	break;

        case 220u:
            SERIAL_msg_handler();
        	break;

        case 240u:
            RFM69_receive_frame();
        	break;

        case 260u:
        	break;

        case 280u:
        	NVM_tick();
        	RFM69_receive_frame();
        	break;

        case 300u:
        	break;

        case 320u:
            SERIAL_msg_handler();
            RFM69_receive_frame();
        	break;

        case 340u:
        	break;

        case 360u:
            RFM69_receive_frame();
        	break;

        case 380u:
        	NVM_tick();
        	break;

        case 400u:
            RFM69_receive_frame();
        	break;

        case 420u:
            SERIAL_msg_handler();
        	break;

        case 440u:
            RFM69_receive_frame();
        	break;

        case 460u:
        	break;

        case 480u:
        	NVM_tick();
        	RFM69_receive_frame();
        	break;

        case 500u:
        	break;

        case 520u:
            SERIAL_msg_handler();
            RFM69_receive_frame();
        	break;

        case 540u:
        	break;

        case 560u:
            RFM69_receive_frame();
        	break;

        case 580u:
        	NVM_tick();
        	break;

        case 600u:
            RFM69_receive_frame();
        	break;

        case 620u:
            SERIAL_msg_handler();
        	break;

        case 640u:
            RFM69_receive_frame();
        	break;

        case 660u:
        	break;

        case 680u:
        	NVM_tick();
        	RFM69_receive_frame();
        	break;

        case 700u:
        	break;

        case 720u:
            SERIAL_msg_handler();
            RFM69_receive_frame();
        	break;

        case 740u:
        	break;

        case 760u:
            RFM69_receive_frame();
        	break;

        case 780u:
        	NVM_tick();
        	break;

        case 800u:
            RFM69_receive_frame();
        	break;

        case 820u:
            SERIAL_msg_handler();
        	break;

        case 840u:
            RFM69_receive_frame();
        	break;

        case 860u:

        	break;

        case 880u:
        	NVM_tick();
        	RFM69_receive_frame();
        	break;

        case 900u:
        	break;

        case 920u:
            SERIAL_msg_handler();
            RFM69_receive_frame();
        	break;

        case 940u:
        	break;

        case 960u:
            RFM69_receive_frame();
        	break;

        case 980u:
        	NVM_tick();
        	break;

        case 1000u:
            RFM69_receive_frame();

            /* keep track of time in secs */
            HAL_TIM_increment_secs();

            /* Trigger the stream */
            SERIAL_trigger_stream_output();
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
	MODE_MGR_system_init = state;
}


