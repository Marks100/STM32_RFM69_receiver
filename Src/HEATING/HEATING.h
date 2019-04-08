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


typedef struct
{
	float oat_c;
	s16_t cool_min_temp_c;
	s16_t cool_max_temp_c;
	s16_t heat_min_temp_c;
	s16_t heat_max_temp_c;
	false_true_et heat_mode;
	disable_enable_et enabled;
	float hysteresis;
} HEATING_room_temp_mon_st;


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void  HEATING_init( void );
void  HEATING_tick( void );
void  HEATING_set_oat( float temperature );
float HEATING_get_oat( void );


#endif /* HEATING_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
