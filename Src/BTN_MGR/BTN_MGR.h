/*! \file
*
*               $Author: mstewart $
*
*   \brief      Button Manager module.
*/
#ifndef BTN_MGR_H
#define BTN_MGR_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

#define BTN_MGR_TICK_TIME_MS      50
#define BTN_MGR_DAB_TIME 	      5u
#define BTN_MGR_HOLD_1_SEC_TIME   ( 1000u / BTN_MGR_TICK_TIME_MS )
#define BTN_MGR_HOLD_2_SEC_TIME   ( 2000u / BTN_MGR_TICK_TIME_MS )
#define BTN_MGR_HOLD_3_SEC_TIME   ( 3000u / BTN_MGR_TICK_TIME_MS )


#define BTN_MGR_PRESS_SHORT_TIME  1u                                 //50ms time
#define BTN_MGR_PRESS_LONG_TIME   ( 2000 / BTN_MGR_TICK_TIME_MS )   //2sec time



/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/

/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/


typedef enum
{
	BTN_MGR_NOT_PRESSED  = 0,
	BTN_MGR_SHORT_PRESS,
	BTN_MGR_LONG_PRESS,
} BTN_MGR_state_et;

typedef enum
{
	BTN_MGR_ROTARY_BUTTON = 0u,
    BTN_MGR_ONBOARD_BUTTON,
	BTN_MGR_NUM_BUTTONS
} BTN_MGR_type_et;


typedef struct
{
    false_true_et    button_pushed;
    BTN_MGR_state_et state;
    BTN_MGR_state_et prev_button_state;
    u16_t            hold_time_ms;
    false_true_et    valid_press;
} BTN_MGR_control_st;
/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void          BTN_MGR_init( void ) ;
void          BTN_MGR_tick( void ) ;
false_true_et BTN_MGR_read_button_state( BTN_MGR_type_et button ) ;
void          BTN_MGR_analyze_hold_times( void ) ;
false_true_et BTN_MGR_check_for_valid_press( void );
void          BTN_MGR_analyze_presses( void );
false_true_et BTN_MGR_adjust_button_times( void );
false_true_et BTN_MGR_wait_for_all_buttons_low( void );
void          BTN_MGR_reset_switches( void );
void          BTN_MGR_carryout_request( void );

#endif /* BTN_MGR multiple inclusion guard */

/****************************** END OF FILE *******************************************************/

