/*! \file
*
*               $Author: mstewart $
*
*   \brief      Button Manager module.
*/

#include "BTN_MGR.h"
#include "C_defs.h"
#include "STDC.h"
#include "HAL_BRD.h"
#include "RF_MGR.h"
#include "NVM.h"




/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/

STATIC BTN_MGR_control_st BTN_MGR_control_s[ BTN_MGR_NUM_BUTTONS ];
STATIC false_true_et      BTN_MGR_active_s = FALSE;

/*!
****************************************************************************************************
*
*   \brief         Module (re-)initialisation function of the buttons
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_init( void )
{
    /* Reset the button control info */
    STDC_memset( &BTN_MGR_control_s, 0x00, sizeof ( BTN_MGR_control_s ) );
    BTN_MGR_active_s = FALSE;
}



/*!
****************************************************************************************************
*
*   \brief        10ms Tick
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_tick( void )
{
	/* Read all the buttons */
	BTN_MGR_read_all_buttons();

    if( BTN_MGR_active_s == FALSE )
    {
        /* now check the button hold time */
        BTN_MGR_analyze_hold_times();

        /* Now try and work out what button combinations are being pressed */
        BTN_MGR_analyze_presses();
    }
    else
    {
        /* Go and carry out the action now that the button(s) have been deemed valid */
        BTN_MGR_carryout_request();

        /* Now that we have managed the button press, lets reset the states so that we
        can go again the next time */
        BTN_MGR_reset_switches();

        /* A button(s) has been deemed valid so Wait until all the buttons have
        been released before letting the user press any more buttons */
        if( BTN_MGR_wait_for_all_buttons_low() == TRUE )
        {
        	BTN_MGR_active_s = FALSE;
        }
    }
}



/*!
****************************************************************************************************
*
*   \brief         Read the physical state of the button
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
false_true_et BTN_MGR_read_button_state( BTN_MGR_type_et button )
{
    /* Read the state of the requested button */
    false_true_et returnType;

    switch( button )
    {
        case BTN_MGR_ROTARY_BUTTON:
        break;

        case BTN_MGR_ONBOARD_BUTTON:
            returnType = HAL_BRD_read_Onboard_btn_state();

            /* The onboard button is inverse logic so lets invert this */
            returnType ^= TRUE;
        break;

      default:
        break;
    }

    return ( returnType  );
}



/*!
****************************************************************************************************
*
*   \brief         Read all the buttons
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_read_all_buttons( void )
{
    u8_t i = 0u;

	/* Check to see if any of the buttons have been pressed */
	for ( i = 0u; i < BTN_MGR_NUM_BUTTONS; i++ )
	{
		/* Read the physical state of each button */
		BTN_MGR_control_s[i].button_pushed = BTN_MGR_read_button_state( (BTN_MGR_type_et)i );

		/* check to see if any of the buttons are HIGH*/
		if( BTN_MGR_control_s[i].button_pushed == TRUE )
		{
			/* The button is HIGH, increment the hold time */
			BTN_MGR_control_s[i].hold_time_ms ++;
		}
	}
}




/*!
****************************************************************************************************
*
*   \brief         Analyze the hold time of the BTN_MGR
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_analyze_hold_times( void )
{
    u8_t i = 0u;

    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        if( ( BTN_MGR_control_s[i].hold_time_ms > BTN_MGR_PRESS_SHORT_TIME ) && ( BTN_MGR_control_s[i].hold_time_ms <= BTN_MGR_PRESS_LONG_TIME  ) )
        {
             BTN_MGR_control_s[i].state = BTN_MGR_SHORT_PRESS;
        }
        else if( BTN_MGR_control_s[i].hold_time_ms >= BTN_MGR_PRESS_LONG_TIME )
        {
             BTN_MGR_control_s[i].state = BTN_MGR_LONG_PRESS;
        }
        else
        {
            BTN_MGR_control_s[i].state = BTN_MGR_NOT_PRESSED;
        }
    }
}



/*!
****************************************************************************************************
*
*   \brief         Analyze the button presses
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_analyze_presses( void )
{
    u8_t i = 0u;
    false_true_et check = FALSE;

    /* we want to wait until the user has released the button before we carry out any action for a short press,
    but not for a long press ( we want to action them straight away ) */
    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
		if( BTN_MGR_control_s[i].state == BTN_MGR_SHORT_PRESS )
		{
	        if( BTN_MGR_control_s[i].button_pushed == FALSE )
	        {
	        	/* The button has been pressed and now has been released */
	        	BTN_MGR_control_s[i].valid_press = TRUE;

	        	/* We will act on the first button that matches the criteria */
	        	BTN_MGR_active_s = TRUE;
	        	break;
	        }
		}
		else if( BTN_MGR_control_s[i].state == BTN_MGR_LONG_PRESS )
		{
			/* We have at least one button long press thats valid */
			BTN_MGR_active_s = TRUE;
			check = TRUE;
		}
    }

	if( check == TRUE )
	{
		/* Now check to see if there are any other buttons that are close to this
		in terms of time pressed because its almost impossible to press multiple buttons at the same time,
		So if any other BTN_MGR is within an acceptable range then we will make it a long press aswell */
		for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
		{
			if( BTN_MGR_control_s[i].button_pushed == TRUE )
			{
				if( ( BTN_MGR_PRESS_LONG_TIME - BTN_MGR_control_s[i].hold_time_ms ) <= BTN_MGR_TIME_ALLOWANCE )
				{
					BTN_MGR_control_s[i].hold_time_ms = BTN_MGR_PRESS_LONG_TIME;
					BTN_MGR_control_s[i].state = BTN_MGR_LONG_PRESS;
					BTN_MGR_control_s[i].valid_press = TRUE;
				}
			}
		}
	}
}



/*!
****************************************************************************************************
*
*   \brief         Check to make sure all buttons are not pressed
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
false_true_et BTN_MGR_wait_for_all_buttons_low( void )
{
    false_true_et returnType = TRUE;
    u8_t i = 0u;
    false_true_et BTN_MGR_button_pushed;

    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        BTN_MGR_button_pushed = BTN_MGR_read_button_state( (BTN_MGR_type_et)i );

        if( BTN_MGR_button_pushed == TRUE )
        {
            /* One of the buttons is not loW  */
            returnType = FALSE;
            break;
        }
    }

    return ( returnType ) ;
}







/*!
****************************************************************************************************
*
*   \brief
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_reset_switches( void )
{
    /* Reset the button control info */
    STDC_memset( &BTN_MGR_control_s, 0x00, sizeof ( BTN_MGR_control_s ) );
}



/*!
****************************************************************************************************
*
*   \brief
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BTN_MGR_carryout_request( void )
{
    /* Lets action the button presses, We may have a single button or multiple
    buttons to process */
	u32_t button_mask = 0;
	u8_t num_long_presses = 0u;
    u8_t i;
    false_true_et action = FALSE;

    /* Lets start with short button presses and we will only allow single short presses */
    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        if( BTN_MGR_control_s[i].valid_press == TRUE )
        {
        	action = TRUE;

            if( BTN_MGR_control_s[i].state == BTN_MGR_SHORT_PRESS )
            {
            	button_mask = i;
            }
            else if( BTN_MGR_control_s[i].state == BTN_MGR_LONG_PRESS )
            {
            	button_mask |= ( 1 << i );
            	num_long_presses += 1;
            }
        }
    }

    if( action == TRUE )
    {
    	if( num_long_presses == 1u )
		{
    		switch( button_mask >> 1 )
			{
				/* This is the short press manager */
				case BTN_MGR_ROTARY_BUTTON:
				break;

				case BTN_MGR_ONBOARD_BUTTON:
				break;

				default:
				break;
			}
		}
		else if( num_long_presses > 1u )
		{
			switch( button_mask )
			{
				/* This is the short press manager */
				case 3:
				break;

				case BTN_MGR_ONBOARD_BUTTON:
				break;

				default:
				break;
			}
		}
		else
		{
			switch( button_mask )
			{
				/* This is the short press manager */
				case BTN_MGR_ROTARY_BUTTON:
				break;

				case BTN_MGR_ONBOARD_BUTTON:
					RF_MGR_set_state( RF_MGR_TX );
				break;

				default:
				break;
			}
		}
    }
}

