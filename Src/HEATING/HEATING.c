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
#include "C_defs.h"
#include "STDC.h"
#include "COMPILER_defs.h"
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

void HEATIING_init( void )
{
	HEATING_room_temp_mon_s.oat_c = TMPERATURE_INVALID;
	HEATING_room_temp_mon_s.heat_mode = TRUE;
	HEATING_room_temp_mon_s.cool_max_temp_c = 19;
	HEATING_room_temp_mon_s.cool_min_temp_c = 15;
	HEATING_room_temp_mon_s.heat_max_temp_c = 20;
	HEATING_room_temp_mon_s.heat_min_temp_c = 18;
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
			if( HEATING_room_temp_mon_s.heat_mode == TRUE )
			{

				/* We are in heat mode */
				if( HEATING_room_temp_mon_s.oat_c < HEATING_room_temp_mon_s.heat_min_temp_c )
				{
					/* Start heating */
					HAL_BRD_set_heater_state( ON );
				}
				else if ( HEATING_room_temp_mon_s.oat_c > HEATING_room_temp_mon_s.heat_max_temp_c )
				{
					/* Stop heating */
					HAL_BRD_set_heater_state( OFF );
				}
				else
				{
					/* temperature is in the expected range... Do nothing */
				}
			}
			else
			{
				/* We are in Cool mode */
				if( HEATING_room_temp_mon_s.oat_c > HEATING_room_temp_mon_s.cool_max_temp_c )
				{
					/* Start cooling */
					HAL_BRD_set_cooler_state( ON );
				}
				else if ( HEATING_room_temp_mon_s.oat_c < HEATING_room_temp_mon_s.cool_min_temp_c )
				{
					/* Stop cooling */
					HAL_BRD_set_cooler_state( OFF );
				}
				else
				{
					/* temperature is in the expected range... Do nothing */
				}
			}
		}
		else
		{
			/* temperature is invalid so do something safe */
			HAL_BRD_set_cooler_state( OFF );
			HAL_BRD_set_heater_state( OFF );
		}
	}
}



void HEATING_set_oat( float temperature )
{
	HEATING_room_temp_mon_s.oat_c = temperature;
}


float HEATING_get_oat( void )
{
	return ( HEATING_room_temp_mon_s.oat_c );
}


