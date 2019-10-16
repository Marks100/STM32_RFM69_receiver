#ifndef ROTARY_H
#define ROTARY_H



/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None */
typedef enum
{
    ROTARY_LEFT_SCROLL = 0u,
    ROTARY_RIGHT_SCROLL,
    ROTARY_SHORT_PPRESS,
    ROTARY_LONG_PPRESS,
    ROTARY_NO_CHANGE
} ROTARY_scroll_type_et;


/***************************************************************************************************
**                              Function Declarations                                             **
***************************************************************************************************/
/* None */
void        ROTARY_init( void );
void        ROTARY_tick( void );
void        ROTARY_debounce_completed( void );
void        ROTARY_evaluate_signals( low_high_et clock, low_high_et data );
void        ROTARY_set_ROTARY_interrupt_state( disable_enable_et state );
void        ROTARY_set_prev_clk_pin_state( low_high_et state );
low_high_et ROTARY_read_rotary_data_pin( void );
low_high_et ROTARY_read_rotary_clock_pin( void );
low_high_et ROTARY_read_rotary_btn_pin( void );
void        ROTARY_increase_glitch_cnt( void );



#endif
