/*! \file
*               $Revision:  $
*
*               $Author: fkelly $
*
*               $Date: $
*
*               $HeadURL: $
*
*   \brief      Public header file for Membrane module.
*/
/* COPYRIGHT NOTICE
* ==================================================================================================
*
* The contents of this document are protected under copyright and contain commercially and/or
* technically confidential information. The content of this document must not be used other than
* for the purpose for which it was provided nor may it be disclosed or copied (by the authorised
* recipient or otherwise) without the prior written consent of an authorised officer of Schrader
* Electronics Ltd.
*
*         (C) $Date:: 2012#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/
#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

//These buttons have been configured off SMCLK which is set to 4Mhz, it also uses TIMERA set to roll over with interrupt
//at ~10ms intervals.  Therefore 100 roll overs will equal approx 1sec
#define BUTTON_DAB_TIME 	     5u
#define BUTTON_HOLD_1_SEC_TIME   100u
#define BUTTON_HOLD_2_SEC_TIME   200u
#define BUTTON_HOLD_3_SEC_TIME   300u

#define BUTTON_TICK_TIME_MS      10
#define BUTTON_PRESS_SHORT_TIME  1     //20ms time
#define BUTTON_PRESS_LONG_TIME   100   //2sec time



/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/

/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/


typedef enum
{
	BUTTON_NOT_PRESSED  = 0,
	BUTTON_SHORT_PRESS,
	BUTTON_LONG_PRESS,
} button_state_et;

typedef enum
{
	ROTARY_BUTTON = 0u,
	NUM_BUTTONS
} button_type_et;


typedef struct
{
    false_true_et button_pushed;
    button_state_et button_state;
    button_state_et prev_button_state;
    u16_t button_hold_time_ms;
    false_true_et valid_press;
} button_control_st;
/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void          BUTTON_init( void ) ;
void          BUTTON_tick( void ) ;
false_true_et BUTTON_read_button_state( button_type_et button ) ;
void          BUTTON_analyze_hold_times( void ) ;
false_true_et BUTTON_check_for_valid_press( void );
void          BUTTON_analyze_presses( void );
false_true_et BUTTON_adjust_button_times( void );
false_true_et BUTTON_wait_for_all_buttons_low( void );
void          BUTTON_reset_switches( void );
void          BUTTON_carryout_request( void );

#endif /* MEMBRANE_PUB_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/

