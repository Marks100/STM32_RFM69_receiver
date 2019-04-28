/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL:
*
*   \brief      RF module
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

#include "PROJ_config.h"
#include "STDC.h"
#include "HAL_BRD.h"
#include "main.h"
#include "nvm.h"
#include "HEATING.h"


#define STDC_MODULE_ID   STDC_MOD_RF_DECODE



extern NVM_info_st NVM_info_s;
STATIC HEATING_room_temp_mon_st HEATING_room_temp_mon_s;

/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/

void HEATING_init( void )
{
	HEATING_room_temp_mon_s.oat_c = TMPERATURE_INVALID;
	HEATING_room_temp_mon_s.heat_mode = HEATING_COOL_MODE;
	HEATING_room_temp_mon_s.cool_max_temp_c = 19;
	HEATING_room_temp_mon_s.cool_min_temp_c = 15;
	HEATING_room_temp_mon_s.heat_max_temp_c = 19.2;
	HEATING_room_temp_mon_s.heat_min_temp_c = 18.0;
	HEATING_room_temp_mon_s.enabled = ENABLE_;
}



void HEATING_tick( void )
{
	/* Check that the feature is enabled */
	if( HEATING_room_temp_mon_s.enabled == ENABLE_ )
	{
		if( HEATING_room_temp_mon_s.oat_c != TMPERATURE_INVALID )
		{
			/* Now check the mode that we want to be in */
			switch( HEATING_room_temp_mon_s.heat_mode )
			{
				case HEATING_HEAT_MODE:
				{
					/* We are in heat mode */
					if( HEATING_room_temp_mon_s.oat_c < HEATING_room_temp_mon_s.heat_min_temp_c )
					{
						/* Start heating */
						HEATING_set_heater_state( ON );
					}
					else if ( HEATING_room_temp_mon_s.oat_c > HEATING_room_temp_mon_s.heat_max_temp_c )
					{
						/* Stop heating */
						HEATING_set_heater_state( OFF );
					}
					else
					{
						/* temperature is in the expected range... Do nothing */
					}
				}
				break;

				case HEATING_COOL_MODE:
				{
					if( HEATING_room_temp_mon_s.oat_c > HEATING_room_temp_mon_s.cool_max_temp_c )
					{
						/* Start cooling */
						HEATING_set_heater_state( ON );
					}
					else if ( HEATING_room_temp_mon_s.oat_c < HEATING_room_temp_mon_s.cool_min_temp_c )
					{
						/* Stop cooling */
						HEATING_set_heater_state( OFF );
					}
					else
					{
						/* temperature is in the expected range... Do nothing */
					}
				}
				break;

				case HEATING_OFF_MODE:
				{
					HEATING_set_heater_state( DISABLE );
					HEATING_set_cooler_state( DISABLE );
				}
				break;

				default:
					break;
			}
		}
		else
		{
			/* temperature is invalid so do something safe */
			HEATING_set_heater_state( OFF );
			HEATING_set_cooler_state( OFF );
		}
	}
	HEATING_update_outputs();
}


void HEATING_set_mode( HEATING_mode_et mode )
{
	HEATING_room_temp_mon_s.heat_mode = mode;
}

HEATING_mode_et HEATING_get_mode( void )
{
	return ( HEATING_room_temp_mon_s.heat_mode );
}

void HEATING_set_state( disable_enable_et state )
{
	HEATING_room_temp_mon_s.enabled = state;
}

disable_enable_et HEATING_get_state( void )
{
	return ( HEATING_room_temp_mon_s.enabled );
}

void HEATING_set_oat( float temperature )
{
	HEATING_room_temp_mon_s.oat_c = temperature;
}


float HEATING_get_oat( void )
{
	return ( HEATING_room_temp_mon_s.oat_c );
}


void HEATING_set_heater_state( disable_enable_et state )
{
	HEATING_room_temp_mon_s.heater_state = state;
}

void HEATING_set_cooler_state( disable_enable_et state )
{
	HEATING_room_temp_mon_s.cooler_state = state;
}




/*
 * LED 0 - system enabled or disabled
 * LED 1 - HEAT MODE ENABLED
 * LED 2 - COOL MODE ENABLED
 * LED 3 - HEATER OR COOLER CURRENTLY RUNNING
 */
void HEATING_update_outputs( void )
{
	/* Set the LED to indicate if the system is enabled or disabled */
	if( HEATING_room_temp_mon_s.enabled == ENABLE_ )
	{
		HAL_BRD_set_LED_state( LED_0, HIGH );

		/* Set the LED to indicate if the system is in heat or cool mode */
		switch( HEATING_get_mode() )
		{
			case HEATING_COOL_MODE:
			{
				HAL_BRD_set_LED_state( LED_1, LOW );
				HAL_BRD_set_LED_state( LED_2, HIGH );
			}
			break;

			case HEATING_HEAT_MODE:
			{
				HAL_BRD_set_LED_state( LED_1, HIGH );
				HAL_BRD_set_LED_state( LED_2, LOW );
			}
			break;

			case HEATING_OFF_MODE:
			{
				HAL_BRD_set_LED_state( LED_1, LOW );
				HAL_BRD_set_LED_state( LED_2, LOW );
				HAL_BRD_set_LED_state( LED_3, LOW );
			}
			break;

			default:
				HAL_BRD_set_LED_state( LED_0, HIGH );
				HAL_BRD_set_LED_state( LED_0, LOW );
			break;
		}

		if( ( HEATING_room_temp_mon_s.heater_state == ENABLE_ ) || ( HEATING_room_temp_mon_s.cooler_state == ENABLE_ ) )
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
}


