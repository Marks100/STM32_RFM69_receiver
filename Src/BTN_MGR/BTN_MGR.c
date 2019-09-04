/*! \file
*
*               $Author: mstewart $
*
*   \brief      Button Manager module.
*/

#ifdef GCC_TEST
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
#endif // GCC_TEST

#include "BTN_MGR.h"
#include "COMPILER_defs.h"
#include "C_defs.h"
#include "STDC.h"
#include "HAL_BRD.h"
#include "MODE_MGR.h"
#include "AIRCON.h"
#include "LCD.h"
#include "NVM.h"




/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/



STATIC BTN_MGR_control_st BTN_MGR_control_s[ BTN_MGR_NUM_BUTTONS ];
STATIC false_true_et      BTN_MGR_valid_flag_s = FALSE;

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
    BTN_MGR_valid_flag_s = FALSE;
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
    u8_t i = 0u;

    if( BTN_MGR_valid_flag_s == FALSE )
    {
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
            BTN_MGR_valid_flag_s = FALSE;
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
        	returnType = HAL_BRD_read_rotary_SW_pin();

        	/* The rotary encoder button is inverse logic so lets invert this */
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

    /* we want to wait until the user has released the button before we carry out any action for a short press,
    but not for a long press ( we want to action them straight away */

    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        /* Read the physical state of each button */
        BTN_MGR_control_s[i].button_pushed = BTN_MGR_read_button_state( (BTN_MGR_type_et)i );

        if( BTN_MGR_control_s[i].button_pushed == FALSE )
        {
            if( BTN_MGR_control_s[i].state == BTN_MGR_SHORT_PRESS )
            {
                /* The button has been pressed and now has been released */
                BTN_MGR_control_s[i].valid_press = TRUE;

                BTN_MGR_valid_flag_s = TRUE;

                /* We will act on the first button that matches the criteria */
                break;
            }
            else if ( BTN_MGR_control_s[i].state == BTN_MGR_NOT_PRESSED )
            {
                 /* The button press is not valid and too short so lets reset the status */
                 BTN_MGR_control_s[i].valid_press = FALSE;
                 BTN_MGR_control_s[i].hold_time_ms = 0u;

                 BTN_MGR_valid_flag_s = FALSE;
            }
        }
    }

    if( BTN_MGR_valid_flag_s == FALSE )
    {
        for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
        {
            if ( BTN_MGR_control_s[i].state == BTN_MGR_LONG_PRESS )
            {
                /* This is where it gets a bit trickier as we now have to manage individual long presses
                and also multi button presses */

               /* The button has been pressed for a long time so it is a valid long press */
               BTN_MGR_control_s[i].valid_press = TRUE;

               /* Now check to see if there are any other buttons that are close to this
               in terms of time pressed because its almost impossible to press multiple buttons at the same time,
               So if any other BTN_MGR is within an acceptable range then we will make it a long press aswell */
               //check for other long presses
               BTN_MGR_adjust_button_times();

               BTN_MGR_valid_flag_s = TRUE;

               /* We will act on the first button that nmatches the criteria */
               break;
            }
        }
    }
    else
    {
        /* We already have a valid button press so dont do any more checking */
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
false_true_et BTN_MGR_adjust_button_times( void )
{
    false_true_et returnType = TRUE;
    u8_t i = 0u;
    u16_t longest_button_hold_time = 0;

    longest_button_hold_time = BTN_MGR_control_s[i].hold_time_ms;

    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        /* Track the longest hold time */
        if( BTN_MGR_control_s[i].hold_time_ms > longest_button_hold_time )
        {
            longest_button_hold_time = BTN_MGR_control_s[i].hold_time_ms;
        }
    }

    /* now check if any of the buttons are within range of the longest hold time */
    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        /* Track the longest hold time */
        if( longest_button_hold_time - BTN_MGR_control_s[i].hold_time_ms < 50 )
        {
            BTN_MGR_control_s[i].hold_time_ms = longest_button_hold_time;
            BTN_MGR_control_s[i].state = BTN_MGR_LONG_PRESS;
            BTN_MGR_control_s[i].valid_press = TRUE;
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
	u8_t button_mask = 0;
    u8_t i;

    /* Lets start with short button presses and we will only allow single short presses */
    for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
    {
        if( BTN_MGR_control_s[i].state == BTN_MGR_SHORT_PRESS )
        {
            if(  BTN_MGR_control_s[i].valid_press == TRUE )
            {
                button_mask |= ( BTN_MGR_control_s[i].valid_press << i );
            }
        }
    }

    if( button_mask != 0 )
    {
        switch( button_mask )
        {
            /* This is the short press manager */
            case 0x01: //ROTARY_BUTTON:
            	SCREENS_handle_rotary_input( ROTARY_SHORT_PPRESS );
                AIRCON_handle_rotary_input( ROTARY_SHORT_PPRESS );
            break;

            default:
            break;
        }
    }


    if( button_mask == 0 )
    {
        /* If the button mask is still 0 it means that either we didnt have any short presses
        or they arent valid, so now check for long presses */
        for ( i = 0; i < BTN_MGR_NUM_BUTTONS; i++ )
        {
            if( BTN_MGR_control_s[i].state == BTN_MGR_LONG_PRESS )
            {
                if(  BTN_MGR_control_s[i].valid_press == TRUE )
                {
                    button_mask |= ( BTN_MGR_control_s[i].valid_press << i );
                }
            }
        }

        if( button_mask != 0 )
        {
            switch( button_mask )
            {
                /* This is the long press manager, so Lots of combinations to do lots of different
                things :) */

                case 0x01: //ROTARY_BUTTON:
                	SCREENS_handle_rotary_input( ROTARY_LONG_PPRESS );
                break;

                default:
                break;
            }
        }
    }
}



