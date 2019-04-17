/*! \file
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/HAL/RF/RF.h $
*
*   \brief      Heating control module.
*/
#ifndef HEATING_H
#define HEATING_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"


#define TMPERATURE_INVALID	( 65535.0f )


typedef enum
{
	HEATING_OFF_MODE = 0u,
	HEATING_HEAT_MODE,
	HEATING_COOL_MODE

} HEATING_mode_et ;

typedef struct
{
	float oat_c;
	float cool_min_temp_c;
	float cool_max_temp_c;
	float heat_min_temp_c;
	float heat_max_temp_c;
	HEATING_mode_et heat_mode;
	disable_enable_et enabled;
	float hysteresis;
	disable_enable_et cooler_state;
	disable_enable_et heater_state;
} HEATING_room_temp_mon_st;


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void  			HEATING_init( void );
void  			HEATING_tick( void );
void  			HEATING_set_oat( float temperature );
float 			HEATING_get_oat( void );
void  		    HEATING_set_mode( HEATING_mode_et mode );
HEATING_mode_et HEATING_get_mode( void );
void 			HEATING_update_outputs( void );
void 			HEATING_set_heater_state( disable_enable_et state );
void 			HEATING_set_cooler_state( disable_enable_et state );


#endif /* HEATING_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
