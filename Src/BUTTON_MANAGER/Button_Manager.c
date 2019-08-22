#ifndef UNIT_TEST

#endif

#ifdef GCC_TEST
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
#endif // GCC_TEST

#include "Button_Manager.h"
#include "COMPILER_defs.h"
#include "C_defs.h"
#include "STDC.h"
#include "HAL_BRD.h"
#include "MODE_MGR.h"
#include "LCD.h"
#include "NVM.h"




/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/



STATIC button_control_st button_control_s[ NUM_BUTTONS ];
STATIC false_true_et button_valid_flag_s = FALSE;

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
void BUTTON_init( void )
{
    /* Reset the button control info */
    STDC_memset( &button_control_s, 0x00, sizeof ( button_control_s ) );
    button_valid_flag_s = FALSE;
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
void BUTTON_tick( void )
{
    u8_t i = 0u;

    if( button_valid_flag_s == FALSE )
    {
        /* Check to see if any of the buttons have been pressed */
        for ( i = 0u; i < NUM_BUTTONS; i++ )
        {
            /* Read the physical state of each button */
            button_control_s[i].button_pushed = BUTTON_read_button_state( (button_type_et)i );

            /* check to see if any of the buttons are HIGH*/
            if(  button_control_s[i].button_pushed == TRUE )
            {
                /* The button is HIGH, increment the hold time */
                button_control_s[i].button_hold_time_ms ++;
            }
        }

         /* now check the button hold time */
         BUTTON_analyze_hold_times();

         /* Now try and work out what button combinations are being pressed */
         BUTTON_analyze_presses();
    }
    else
    {

        /* Go and carry out the action now that the button(s) have been deemed valid */
        BUTTON_carryout_request();

        /* Now that we have managed the button press, lets reset the states so that we
        can go again the next time */
        BUTTON_reset_switches();

        /* A button(s) has been deemed valid so Wait until all the buttons have
        been released before letting the user press any more buttons */
        if( BUTTON_wait_for_all_buttons_low() == TRUE )
        {
            button_valid_flag_s = FALSE;
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
false_true_et BUTTON_read_button_state( button_type_et button )
{
    /* Read the state of the requested button */
    false_true_et returnType;

    switch( button )
    {
        case ROTARY_BUTTON:
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
*   \brief         Analyze the hold time of the button
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void BUTTON_analyze_hold_times( void )
{
    u8_t i = 0u;

    for ( i = 0; i < NUM_BUTTONS; i++ )
    {
        if( ( button_control_s[i].button_hold_time_ms > BUTTON_PRESS_SHORT_TIME ) && ( button_control_s[i].button_hold_time_ms <= BUTTON_PRESS_LONG_TIME  ) )
        {
             button_control_s[i].button_state = BUTTON_SHORT_PRESS;
        }
        else if( button_control_s[i].button_hold_time_ms >= BUTTON_PRESS_LONG_TIME )
        {
             button_control_s[i].button_state = BUTTON_LONG_PRESS;
        }
        else
        {
             button_control_s[i].button_state = BUTTON_NOT_PRESSED;
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
void BUTTON_analyze_presses( void )
{
    u8_t i = 0u;

    /* we want to wait until the user has released the button before we carry out any action for a short press,
    but not for a long press ( we want to action them straight away */

    for ( i = 0; i < NUM_BUTTONS; i++ )
    {
        /* Read the physical state of each button */
        button_control_s[i].button_pushed = BUTTON_read_button_state( (button_type_et)i );

        if( button_control_s[i].button_pushed == FALSE )
        {
            if( button_control_s[i].button_state == BUTTON_SHORT_PRESS )
            {
                /* The button has been pressed and now has been released */
                button_control_s[i].valid_press = TRUE;

                button_valid_flag_s = TRUE;

                /* We will act on the first button that matches the criteria */
                break;
            }
            else if ( button_control_s[i].button_state == BUTTON_NOT_PRESSED )
            {
                 /* The button press is not valid and too short so lets reset the status */
                 button_control_s[i].valid_press = FALSE;
                 button_control_s[i].button_hold_time_ms = 0u;

                 button_valid_flag_s = FALSE;
            }
        }
    }

    if( button_valid_flag_s == FALSE )
    {
        for ( i = 0; i < NUM_BUTTONS; i++ )
        {
            if ( button_control_s[i].button_state == BUTTON_LONG_PRESS )
            {
                /* This is where it gets a bit trickier as we now have to manage individual long presses
                and also multi button presses */

               /* The button has been pressed for a long time so it is a valid long press */
               button_control_s[i].valid_press = TRUE;

               /* Now check to see if there are any other buttons that are close to this
               in terms of time pressed because its almost impossible to press multiple buttons at the same time,
               So if any other button is within an acceptable range then we will make it a long press aswell */
               //check for other long presses
               BUTTON_adjust_button_times();

               button_valid_flag_s = TRUE;

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
false_true_et BUTTON_wait_for_all_buttons_low( void )
{
    false_true_et returnType = TRUE;
    u8_t i = 0u;
    false_true_et button_pushed;

    for ( i = 0; i < NUM_BUTTONS; i++ )
    {
        button_pushed = BUTTON_read_button_state( (button_type_et)i );

        if( button_pushed == TRUE )
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
false_true_et BUTTON_adjust_button_times( void )
{
    false_true_et returnType = TRUE;
    u8_t i = 0u;
    u16_t longest_button_hold_time = 0;

    longest_button_hold_time = button_control_s[i].button_hold_time_ms;

    for ( i = 0; i < NUM_BUTTONS; i++ )
    {
        /* Track the longest hold time */
        if( button_control_s[i].button_hold_time_ms > longest_button_hold_time )
        {
            longest_button_hold_time = button_control_s[i].button_hold_time_ms;
        }
    }

    /* now check if any of the buttons are within range of the longest hold time */
    for ( i = 0; i < NUM_BUTTONS; i++ )
    {
        /* Track the longest hold time */
        if( longest_button_hold_time - button_control_s[i].button_hold_time_ms < 50 )
        {
            button_control_s[i].button_hold_time_ms = longest_button_hold_time;
            button_control_s[i].button_state = BUTTON_LONG_PRESS;
            button_control_s[i].valid_press = TRUE;
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
void BUTTON_reset_switches( void )
{
    /* Reset the button control info */
    STDC_memset( &button_control_s, 0x00, sizeof ( button_control_s ) );
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
void BUTTON_carryout_request( void )
{
    /* Lets action the button presses, We may have a single button or multiple
    buttons to process */
	u8_t button_mask = 0;
    u8_t i;

    /* Lets start with short button presses and we will only allow single short presses */
    for ( i = 0; i < NUM_BUTTONS; i++ )
    {
        if( button_control_s[i].button_state == BUTTON_SHORT_PRESS )
        {
            if(  button_control_s[i].valid_press == TRUE )
            {
                button_mask |= ( button_control_s[i].valid_press << i );
            }
        }
    }

    if( button_mask != 0 )
    {
        switch( button_mask )
        {
            /* This is the short press manager */
            case 0x01: //ROTARY_BUTTON:
            	SCREENS_handle_rotary_input(ROTARY_SHORT_PPRESS);
            break;

            default:
            break;
        }
    }


    if( button_mask == 0 )
    {
        /* If the button mask is still 0 it means that either we didnt have any short presses
        or they arent valid, so now check for long presses */
        for ( i = 0; i < NUM_BUTTONS; i++ )
        {
            if( button_control_s[i].button_state == BUTTON_LONG_PRESS )
            {
                if(  button_control_s[i].valid_press == TRUE )
                {
                    button_mask |= ( button_control_s[i].valid_press << i );
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
                	SCREENS_handle_rotary_input(ROTARY_LONG_PPRESS);
                break;

                default:
                break;
            }
        }
    }
}



