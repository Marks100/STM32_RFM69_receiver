/*! \file
*
*               $Author: mstewart $
*
*   \brief      AIRCON control module.
*/
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#ifndef UNIT_TEST
#endif

#ifdef GCC_TEST
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif // GCC_TEST

#include <math.h>
#include "STDC.h"
#include "HAL_BRD.h"
#include "NVM.h"
#include "AIRCON.h"

#define STDC_MODULE_ID   STDC_MOD_RF_DECODE

/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
extern NVM_info_st NVM_info_s;
STATIC AIRCON_config_st  AIRCON_config_s;
STATIC AIRCON_min_max_st AIRCON_min_max_s;
STATIC disable_enable_et AIRCON_generic_outputs_s[MAX_NUM_OUTPUTS];


/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/
void AIRCON_init( void )
{
	AIRCON_config_s.oat_c              = TMPERATURE_NOT_AVAILABLE;
	AIRCON_config_s.mode               = NVM_info_s.NVM_generic_data_blk_s.aircon_mode;
	AIRCON_config_s.cool_target_temp_c = NVM_info_s.NVM_generic_data_blk_s.cool_target_temp;
	AIRCON_config_s.heat_target_temp_c = NVM_info_s.NVM_generic_data_blk_s.heat_target_temp;
	AIRCON_config_s.state              = NVM_info_s.NVM_generic_data_blk_s.aircon_state;
	AIRCON_config_s.auto_target_temp_c = NVM_info_s.NVM_generic_data_blk_s.auto_target_temp;

	AIRCON_min_max_s.max_temp_c.value = TMPERATURE_NOT_AVAILABLE;
	AIRCON_min_max_s.max_temp_c.id    = 0u;
	AIRCON_min_max_s.min_temp_c.value = TMPERATURE_NOT_AVAILABLE;
	AIRCON_min_max_s.min_temp_c.id    = 0u;
	AIRCON_min_max_s.temp_delta_c     = 0u;	
}



void AIRCON_tick( void )
{
	float target;

	/* Check that the feature is enabled */
	if( AIRCON_config_s.state == ENABLE_ )
	{
		if( ( AIRCON_config_s.oat_c != TMPERATURE_NOT_AVAILABLE ) && ( AIRCON_config_s.oat_c < TMPERATURE_INVALID ) )
		{
			/* Now check the mode that we want to be in */
			switch( AIRCON_config_s.mode )
			{
				case AIRCON_HEAT_MODE:
				{
					AIRCON_set_cooler_state( OFF );

					target = AIRCON_config_s.heat_target_temp_c;

					/* We are in heat mode */
					if( AIRCON_config_s.oat_c < ( target - 0.2 ) )
					{
						/* Start heating */
						AIRCON_set_heater_state( ON );
					}
					else if( AIRCON_config_s.oat_c > ( target + 0.2 ) )
					{
						/* Stop heating */
						AIRCON_set_heater_state( OFF );
					}
					else
					{
						/* temperature is in the expected range... Do nothing */
						AIRCON_set_heater_state( OFF );
					}
				}
				break;

				case AIRCON_COOL_MODE:
				{
					AIRCON_set_heater_state( OFF );

					target = AIRCON_config_s.cool_target_temp_c;

					if( AIRCON_config_s.oat_c > ( target + 0.2 ) )
					{
						/* Start cooling */
						AIRCON_set_cooler_state( ON );
					}
					else if ( AIRCON_config_s.oat_c < ( target - 0.2 ) )
					{
						/* Stop cooling */
						AIRCON_set_cooler_state( OFF );
					}
					else
					{
						/* temperature is in the expected range... Do nothing */
						AIRCON_set_cooler_state( OFF );
					}
				}
				break;

				case AIRCON_AUTO_MODE:
				{
					target = AIRCON_config_s.auto_target_temp_c;

					if( AIRCON_config_s.oat_c < ( target - ( target * 0.05 ) ) )
					{
						AIRCON_set_heater_state( ON );
						AIRCON_set_cooler_state( OFF );
					}
					else if( AIRCON_config_s.oat_c > ( target + ( target * 0.05 ) ) )
					{
						AIRCON_set_heater_state( OFF );
						AIRCON_set_cooler_state( ON );
					}
					else
					{
						/* temperature is in the expected range... Do nothing */
						AIRCON_set_heater_state( OFF );
						AIRCON_set_cooler_state( OFF );
					}
				}
				break;

				default:
					break;
			}
		}
		else
		{
			/* temperature is invalid so do something safe */
			AIRCON_set_heater_state( OFF );
			AIRCON_set_cooler_state( OFF );
		}
	}
	else
	{
		/* Disabled so do something safe */
		AIRCON_set_heater_state( OFF );
		AIRCON_set_cooler_state( OFF );
	}
	AIRCON_update_outputs();
	NVM_request_flush();
}


void AIRCON_handle_rotary_input( ROTARY_scroll_type_et type )
{
	if( type == ROTARY_SHORT_PPRESS )
	{
		/* Only save to NVM when the user has interacted with the button */
		NVM_info_s.NVM_generic_data_blk_s.aircon_state     = AIRCON_config_s.state;
		NVM_info_s.NVM_generic_data_blk_s.aircon_mode      = AIRCON_config_s.mode;
		NVM_info_s.NVM_generic_data_blk_s.cool_target_temp = AIRCON_config_s.cool_target_temp_c;
		NVM_info_s.NVM_generic_data_blk_s.heat_target_temp = AIRCON_config_s.heat_target_temp_c;
		NVM_info_s.NVM_generic_data_blk_s.auto_target_temp = AIRCON_config_s.auto_target_temp_c;
	
		NVM_request_flush();
	}
}



void AIRCON_set_mode( AIRCON_mode_et mode )
{
	if( mode < AIRCON_MODE_MAX )
	{
		AIRCON_config_s.mode = mode;
	}
}


AIRCON_mode_et AIRCON_get_mode( void )
{
	return ( AIRCON_config_s.mode );
}


void AIRCON_toggle_mode( void )
{
	if( AIRCON_config_s.mode < AIRCON_AUTO_MODE )
	{
		AIRCON_config_s.mode ++;
	}
	else
	{
		AIRCON_config_s.mode = (AIRCON_mode_et)0u;
	}
}


void AIRCON_set_state( disable_enable_et state )
{
	if( state < DISABLE_ENABLE_MAX )
	{
		AIRCON_config_s.state = state;	
	}
}


disable_enable_et AIRCON_get_state( void )
{
	return ( AIRCON_config_s.state );
}


float AIRCON_get_temperature_setting( AIRCON_mode_et type )
{
	float return_val;

	switch( type )
	{
		case AIRCON_HEAT_MODE:
		{
			return_val = AIRCON_config_s.heat_target_temp_c;
		}
		break;

		case AIRCON_COOL_MODE:
		{
			return_val = AIRCON_config_s.cool_target_temp_c;
		}
		break;

		case AIRCON_AUTO_MODE:
		{
			return_val = AIRCON_config_s.auto_target_temp_c;
		}
		break;

		default:
			break;
	}

	return ( return_val );
}


void AIRCON_adjust_temperature_setting(AIRCON_mode_et type, u8_t dir)
{
	float step_value = 0.5;

	switch( type )
	{
		case AIRCON_HEAT_MODE:
		{
			AIRCON_config_s.heat_target_temp_c = ( dir == SETTING_ADJUST_UP ) ?  \
			( AIRCON_config_s.heat_target_temp_c + step_value ) : \
			( AIRCON_config_s.heat_target_temp_c - step_value );
		}
		break;

		case AIRCON_COOL_MODE:
		{
			AIRCON_config_s.cool_target_temp_c = ( dir == SETTING_ADJUST_UP ) ? \
			( AIRCON_config_s.cool_target_temp_c + step_value) : \
			( AIRCON_config_s.cool_target_temp_c - step_value );
		}
		break;

		case AIRCON_AUTO_MODE:
		{
			AIRCON_config_s.auto_target_temp_c = ( dir == SETTING_ADJUST_UP ) ? \
			( AIRCON_config_s.auto_target_temp_c + step_value ) : \
			( AIRCON_config_s.auto_target_temp_c - step_value );
		}
		break;

		default:
			break;
	}

	AIRCON_validate_temp_settings();
}


void AIRCON_validate_temp_settings( void )
{
	/* Boundary check the temperature settings */
	if( AIRCON_config_s.auto_target_temp_c < MIN_AIRCON_TEMP_C_SETTING )
	{
		AIRCON_config_s.auto_target_temp_c = MIN_AIRCON_TEMP_C_SETTING;
	}
	else if( AIRCON_config_s.auto_target_temp_c > MAX_AIRCON_TEMP_C_SETTING )
	{
		AIRCON_config_s.auto_target_temp_c = MAX_AIRCON_TEMP_C_SETTING;
	}

	if( AIRCON_config_s.cool_target_temp_c < MIN_AIRCON_TEMP_C_SETTING )
	{
		AIRCON_config_s.cool_target_temp_c = MIN_AIRCON_TEMP_C_SETTING;
	}
	else if( AIRCON_config_s.cool_target_temp_c > MAX_AIRCON_TEMP_C_SETTING )
	{
		AIRCON_config_s.cool_target_temp_c = MAX_AIRCON_TEMP_C_SETTING;
	}
	
	if( AIRCON_config_s.heat_target_temp_c < MIN_AIRCON_TEMP_C_SETTING )
	{
		AIRCON_config_s.heat_target_temp_c = MIN_AIRCON_TEMP_C_SETTING;
	}
	else if( AIRCON_config_s.heat_target_temp_c > MAX_AIRCON_TEMP_C_SETTING )
	{
		AIRCON_config_s.heat_target_temp_c = MAX_AIRCON_TEMP_C_SETTING;
	}
}



void AIRCON_toggle_state( void )
{
	if( AIRCON_get_state() == DISABLE_ )
	{
		AIRCON_set_state( ENABLE_ );
	}
	else
	{
		AIRCON_set_state( DISABLE_ );
	}
}


void AIRCON_set_oat( float temperature, u16_t node_id )
{
	if( temperature > MAX_ALLOWED_TEMP )
	{
		AIRCON_config_s.oat_c = TMPERATURE_INVALID;
	}
	else if( temperature < MIN_ALLOWED_TEMP )
	{
		AIRCON_config_s.oat_c = TMPERATURE_INVALID;
	}
	else
	{
		AIRCON_config_s.oat_c = temperature;

		/* Now that the temp is valid lets track max and min */
		if( ( AIRCON_min_max_s.max_temp_c.value > MAX_ALLOWED_TEMP ) || ( AIRCON_min_max_s.min_temp_c.value > MAX_ALLOWED_TEMP ) )
		{
			AIRCON_min_max_s.max_temp_c.value = temperature;
			AIRCON_min_max_s.max_temp_c.id	  = node_id;
			AIRCON_min_max_s.min_temp_c.value = temperature;
			AIRCON_min_max_s.min_temp_c.id	  = node_id;
		}
		else
		{
			if( AIRCON_config_s.oat_c > AIRCON_min_max_s.max_temp_c.value )
			{
				AIRCON_min_max_s.max_temp_c.value = AIRCON_config_s.oat_c;
				AIRCON_min_max_s.max_temp_c.id	  = node_id;
			}
			else if( AIRCON_config_s.oat_c < AIRCON_min_max_s.min_temp_c.value )
			{
				AIRCON_min_max_s.min_temp_c.value = AIRCON_config_s.oat_c;
				AIRCON_min_max_s.min_temp_c.id	  = node_id;
			}
			else
			{
				/* code */
			}
					
		}

		AIRCON_min_max_s.temp_delta_c = ( AIRCON_min_max_s.max_temp_c.value - AIRCON_min_max_s.min_temp_c.value );
	}
}


float AIRCON_get_oat( void )
{
	return ( AIRCON_config_s.oat_c );
}


void AIRCON_set_heater_state( disable_enable_et state )
{
	AIRCON_config_s.heater_state = state;
}


void AIRCON_set_cooler_state( disable_enable_et state )
{
	AIRCON_config_s.cooler_state = state;
}

disable_enable_et AIRCON_get_heater_state( void )
{
	return( AIRCON_config_s.heater_state );
}

disable_enable_et AIRCON_get_cooler_state( void )
{
	return( AIRCON_config_s.cooler_state );
}


void AIRCON_set_generic_output_state( u8_t output_num, disable_enable_et state )
{
	if( output_num < MAX_NUM_OUTPUTS )
	{
		AIRCON_generic_outputs_s[output_num] = state;
	}
}



/*
 * LED 0 - system enabled or disabled
 * LED 1 - HEAT MODE ENABLED
 * LED 2 - COOL MODE ENABLED
 * LED 3 - HEATER OR COOLER CURRENTLY RUNNING
 */
void AIRCON_update_outputs( void )
{
	/* Set the LED to indicate if the system is enabled or disabled */
	if( AIRCON_config_s.state == ENABLE_ )
	{
		HAL_BRD_set_LED_state( LED_0, HIGH );

		/* Set the LED to indicate if the system is in heat or cool mode */
		switch( AIRCON_get_mode() )
		{
			case AIRCON_COOL_MODE:
			{
				HAL_BRD_set_LED_state( LED_1, HIGH );
				HAL_BRD_set_LED_state( LED_2, LOW );
			}
			break;

			case AIRCON_HEAT_MODE:
			{
				HAL_BRD_set_LED_state( LED_1, LOW );
				HAL_BRD_set_LED_state( LED_2, HIGH );
			}
			break;

			case AIRCON_AUTO_MODE:
			{
				HAL_BRD_set_LED_state( LED_1, LOW );
				HAL_BRD_set_LED_state( LED_2, LOW );
			}
			break;

			default:
				HAL_BRD_set_LED_state( LED_0, HIGH );
				HAL_BRD_set_LED_state( LED_0, LOW );
			break;
		}

		if( ( AIRCON_config_s.heater_state == ENABLE_ ) || ( AIRCON_config_s.cooler_state == ENABLE_ ) )
		{
			HAL_BRD_set_LED_state( LED_3, HIGH );
		}
		else
		{
			HAL_BRD_set_LED_state( LED_3, LOW );
		}
	}
	else
	{
		HAL_BRD_set_LED_state( LED_0, LOW );
		HAL_BRD_set_LED_state( LED_1, LOW );
		HAL_BRD_set_LED_state( LED_2, LOW );
		HAL_BRD_set_LED_state( LED_3, LOW );
	}

	/***************************************/
	u8_t i = 0u;

	for( i = 0u; i < MAX_NUM_OUTPUTS; i++ )
	{
		/* Each individual output has a different pin so must be handled individually */
		if( AIRCON_generic_outputs_s[i] == ENABLE_ )
		{
			HAL_BRD_set_generic_output( AIRCON_generic_outputs_s[i], HIGH );
		}
		else
		{
			HAL_BRD_set_generic_output( AIRCON_generic_outputs_s[i], LOW );
		}	
	}
}




AIRCON_min_max_st* AIRCON_get_stats_address( void )
{
	return( &AIRCON_min_max_s );
}
