/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL:
*
*   \brief      AIRCON module
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
#include "nvm.h"
#include "AIRCON.h"


#define STDC_MODULE_ID   STDC_MOD_RF_DECODE



extern NVM_info_st NVM_info_s;
STATIC AIRCON_config_st  AIRCON_config_s;
STATIC AIRCON_min_max_st AIRCON_min_max_s;
STATIC disable_enable_et AIRCON_generic_outputs_s[MAX_NUM_OUTPUTS];

/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/

void AIRCON_init( void )
{
	AIRCON_config_s.oat_c = TMPERATURE_NOT_AVAILABLE;
	AIRCON_config_s.mode = NVM_info_s.NVM_generic_data_blk_s.aircon_mode;
	AIRCON_config_s.cool_max_temp_c = NVM_info_s.NVM_generic_data_blk_s.cool_max_temp;
	AIRCON_config_s.cool_min_temp_c = NVM_info_s.NVM_generic_data_blk_s.cool_min_temp;
	AIRCON_config_s.heat_max_temp_c = NVM_info_s.NVM_generic_data_blk_s.heat_max_temp;
	AIRCON_config_s.heat_min_temp_c = NVM_info_s.NVM_generic_data_blk_s.heat_min_temp;
	AIRCON_config_s.state = NVM_info_s.NVM_generic_data_blk_s.aircon_state;
	AIRCON_config_s.auto_target_temp_c =  NVM_info_s.NVM_generic_data_blk_s.auto_target_temp;

	AIRCON_min_max_s.max_temp_c.value = TMPERATURE_NOT_AVAILABLE;
	AIRCON_min_max_s.max_temp_c.id    = 0u;
	AIRCON_min_max_s.min_temp_c.value = TMPERATURE_NOT_AVAILABLE;
	AIRCON_min_max_s.min_temp_c.id    = 0u;
	AIRCON_min_max_s.temp_delta_c     = 0u;	
}



void AIRCON_tick( void )
{
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

					/* We are in heat mode */
					if( AIRCON_config_s.oat_c < AIRCON_config_s.heat_min_temp_c )
					{
						/* Start heating */
						AIRCON_set_heater_state( ON );
					}
					else if ( AIRCON_config_s.oat_c > AIRCON_config_s.heat_max_temp_c )
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

					if( AIRCON_config_s.oat_c > AIRCON_config_s.cool_max_temp_c )
					{
						/* Start cooling */
						AIRCON_set_cooler_state( ON );
					}
					else if ( AIRCON_config_s.oat_c < AIRCON_config_s.cool_min_temp_c )
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
					float target = AIRCON_config_s.auto_target_temp_c;

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


void AIRCON_set_generic_output_state( u8_t output_num, disable_enable_et state )
{
	if( output_num < MAX_NUM_OUTPUTS )
	{
		AIRCON_generic_outputs_s[output_num] = state;
	}
}

void AIRCON_togggle_generic_output_state( u8_t output_num )
{
	if( output_num < MAX_NUM_OUTPUTS )
	{
		if( AIRCON_get_generic_output_state(output_num) == ENABLE_ )
		{
			AIRCON_set_generic_output_state( output_num, DISABLE_ );
		}
		else
		{
			AIRCON_set_generic_output_state( output_num, ENABLE_ );
		}
	}
}

disable_enable_et AIRCON_get_generic_output_state( u8_t output_num )
{
	if( output_num < MAX_NUM_OUTPUTS )
	{
		return( AIRCON_generic_outputs_s[output_num] );
	}
}



void AIRCON_decode_control_cmd( u8_t cmd )
{
	/* we have just received a control command */


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
				if( AIRCON_config_s.heater_state == ENABLE_ )
				{
					HAL_BRD_set_LED_state( LED_1, LOW );
					HAL_BRD_set_LED_state( LED_2, HIGH );
				}
				else if( AIRCON_config_s.cooler_state == ENABLE_ )
				{
					HAL_BRD_set_LED_state( LED_1, HIGH );
					HAL_BRD_set_LED_state( LED_2, LOW );
				}
				else
				{
					HAL_BRD_set_LED_state( LED_1, LOW );
					HAL_BRD_set_LED_state( LED_2, LOW );
				}
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
