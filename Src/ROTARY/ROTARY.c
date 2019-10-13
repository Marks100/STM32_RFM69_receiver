#include "HAL_BRD.h"
#include "STDC.h"
#include "HAL_TIM.h"


low_high_et ROTARY_rotary_clock_s;
low_high_et ROTARY_rotary_data_s;
low_high_et ROTARY_prev_rotary_clock_s;
low_high_et ROTARY_prev_rotary_data_s;
low_high_et ROTARY_trigger_state_s;
u16_t       ROTARY_plus_cnt_s;
u16_t       ROTARY_minus_cnt_s;


/*!
****************************************************************************************************
*
*   \brief         Initialise the Rotary signals,
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void ROTARY_init( void )
{
	ROTARY_rotary_clock_s = LOW;
	ROTARY_rotary_data_s = LOW;
	ROTARY_prev_rotary_clock_s = LOW;
	ROTARY_prev_rotary_data_s = LOW;
	ROTARY_trigger_state_s = LOW;
	ROTARY_plus_cnt_s = 0u;
	ROTARY_minus_cnt_s = 0u;
}




/*!
****************************************************************************************************
*
*   \brief         Reads the state of the rotary clock pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
low_high_et ROTARY_read_rotary_clock_pin( void )
{
    low_high_et state = LOW;

    state = HAL_BRD_read_rotary_clock_pin();

    return( state );
}

/*!
****************************************************************************************************
*
*   \brief         Reads the state of the rotary data pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
low_high_et ROTARY_read_rotary_data_pin( void )
{
    low_high_et state = LOW;

    state = HAL_BRD_read_rotary_data_pin();

    return( state );
}


/****************************************************************************************************
*
*   \brief         Reads the state of the rotary SW pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
low_high_et ROTARY_read_rotary_SW_pin( void )
{
    low_high_et state = LOW;

    state = HAL_BRD_read_rotary_SW_pin();

    return( state );
}


/****************************************************************************************************
*
*   \brief         Sets the prev state of the rotary data pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
void ROTARY_set_prev_data_pin_state( low_high_et state )
{
    ROTARY_prev_rotary_data_s = state;
}

/****************************************************************************************************
*
*   \brief         Sets the prev state of the rotary clk pin
*
*   \author        MS
*
*   \return        low_high_et state of the pin
*
***************************************************************************************************/
void ROTARY_set_prev_clk_pin_state( low_high_et state )
{
    ROTARY_prev_rotary_clock_s = state;
}


/*!
****************************************************************************************************
*
*   \brief         This gets called after the timer has debounced the LOGIC for the Rotary encoder
*
*   \author        MS
*
*   \return        low_high_et
*
***************************************************************************************************/
void ROTARY_debounce_completed( void )
{
	/* Read the clock and data pin again */
	ROTARY_rotary_data_s = ROTARY_read_rotary_data_pin();
	ROTARY_rotary_clock_s = ROTARY_read_rotary_clock_pin();

	/* Check to make sure that the pin is the same state as it was in the ISR */
	if( ROTARY_prev_rotary_clock_s == ROTARY_rotary_clock_s )
	{
		/* Both the previous state and the current state are the same value and have been
		 * for at least 1ms so this is a valid pin transition
		 */
		if( ROTARY_trigger_state_s == LOW )
		{
			ROTARY_trigger_state_s = HIGH;

			/* Check to make sure that the pin is the same state as it was in the ISR */
			if( ROTARY_rotary_data_s == ROTARY_prev_rotary_data_s )
			{
				ROTARY_evaluate_signals( ROTARY_rotary_clock_s, ROTARY_rotary_data_s );
			}
			else
			{
				/* do nothing */
				//assert(1);
			}
		}
		else
		{
			ROTARY_trigger_state_s = LOW;
		}
	}
	else
	{
		/* The pins are not the same state for at least 1ms so this
		 * is probably a glitch that needs debounced ta fuck */
	}

	ROTARY_set_hw_edge_detection( ROTARY_trigger_state_s );
}


void ROTARY_set_ROTARY_interrupt_state( disable_enable_et state )
{
	HAL_BRD_set_ROTARY_interrupt_state( state );
}

void ROTARY_set_hw_edge_detection( low_high_et state )
{
	HAL_BRD_set_ROTARY_edge_state( ROTARY_trigger_state_s );
}

low_high_et ROTARY_get_hw_edge_detection( void )
{
	return( ROTARY_trigger_state_s );
}






void ROTARY_evaluate_signals( low_high_et clock, low_high_et data )
{
 	if( ( clock == LOW ) && ( data == HIGH ) )
	{
 		ROTARY_minus_cnt_s ++;
	}
	else if( ( clock == LOW ) && ( data == LOW ) )
	{
		ROTARY_plus_cnt_s ++;
	}
}


/*!
****************************************************************************************************
*
*   \brief         This is USER configurable and should call out to all modules that are relevant
*                  to inform them of the current
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void ROTARY_tick( void )
{
	u16_t i = 0u;
	u16_t left = ROTARY_minus_cnt_s;
	u16_t right = ROTARY_plus_cnt_s;

	/* Reset the current counts */
	ROTARY_minus_cnt_s = 0u;
	ROTARY_plus_cnt_s = 0u;

	/* Handle all the left scrolls */
	for( i = 0u; i < left; i++ )
	{
		NEOPIXEL_handle_rotary_input( ROTARY_LEFT_SCROLL );
		SCREENS_handle_rotary_input( ROTARY_LEFT_SCROLL );
	}

	/* Handle all the left scrolls */
	for( i = 0u; i < right; i++ )
	{
		NEOPIXEL_handle_rotary_input( ROTARY_RIGHT_SCROLL );
		SCREENS_handle_rotary_input( ROTARY_RIGHT_SCROLL );
	}
}
