/*! \file
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/HAL/RF/RF.h $
*
*   \brief      AIRCON control module.
*/
#ifndef AIRCON_H
#define AIRCON_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"


#define TMPERATURE_INVALID	( 65535.0f )


typedef enum
{
	AIRCON_HEAT_MODE = 0u,
	AIRCON_COOL_MODE,
	AIRCON_AUTO_MODE

} AIRCON_mode_et ;

typedef struct
{
	float oat_c;
	float cool_min_temp_c;
	float cool_max_temp_c;
	float heat_min_temp_c;
	float heat_max_temp_c;
	float auto_target_temp_c;
	AIRCON_mode_et mode;
	disable_enable_et state;
	float hysteresis;
	disable_enable_et cooler_state;
	disable_enable_et heater_state;
} AIRCON_config_st;


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void  			  AIRCON_init( void );
void  			  AIRCON_tick( void );
void  			  AIRCON_set_oat( float temperature );
float 			  AIRCON_get_oat( void );
void  		      AIRCON_set_mode( AIRCON_mode_et mode );
AIRCON_mode_et    AIRCON_get_mode( void );
void  		      AIRCON_toggle_mode( void );
void 			  AIRCON_update_outputs( void );
void 			  AIRCON_set_heater_state( disable_enable_et state );
void 			  AIRCON_set_cooler_state( disable_enable_et state );
disable_enable_et AIRCON_get_state( void );
void 			  AIRCON_set_state( disable_enable_et state );
void 			  AIRCON_toggle_state( void );
void 			  AIRCON_decode_control_cmd( u8_t cmd );

#endif /* AIRCON_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
