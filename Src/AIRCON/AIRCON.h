/*! \file
*
*               $Author: mstewart $
*
*   \brief      AIRCON control module.
*/
#ifndef AIRCON_H
#define AIRCON_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "HAL_BRD.h"
#include "ROTARY.h"

#define SETTING_ADJUST_UP			( 1u )
#define SETTING_ADJUST_DOWN			( 0u )

#define MAX_ALLOWED_TEMP			( 100.0 )
#define MIN_ALLOWED_TEMP			( -50.0 )
#define TMPERATURE_NOT_AVAILABLE	( 65535.0f )
#define TMPERATURE_INVALID			( 65534.0f )
#define MAX_NUM_OUTPUTS 			( 10 )

#define MIN_AIRCON_TEMP_C_SETTING	( 16.0 )
#define MAX_AIRCON_TEMP_C_SETTING	( 22.5 )
#define MAX_NUM_SETTINGS			( ( MAX_AIRCON_TEMP_C_SETTING - MIN_AIRCON_TEMP_C_SETTING ) * 2.00 )
#define STEP_VALUE 					( 0.5f )

typedef enum AIRCON_mode_et
{
	AIRCON_HEAT_MODE = 0u,
	AIRCON_COOL_MODE,
	AIRCON_AUTO_MODE,
	AIRCON_MODE_MAX

} AIRCON_mode_et ;

typedef struct
{
	float oat_c;
	float cool_target_temp_c;
	float heat_target_temp_c;
	float auto_target_temp_c;
	AIRCON_mode_et mode;
	disable_enable_et state;
	float hysteresis;
	disable_enable_et cooler_state;
	disable_enable_et heater_state;
} AIRCON_config_st;

typedef struct
{
	float value;
	u16_t id;	
} AIRCON_parameter_st;

typedef struct
{
	AIRCON_parameter_st min_temp_c;
	AIRCON_parameter_st max_temp_c;
	float temp_delta_c;
} AIRCON_min_max_st;


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void  			   AIRCON_init( void );
void  			   AIRCON_tick( void );
void  			   AIRCON_set_oat( float temperature, u16_t node_id );
float 			   AIRCON_get_oat( void );
void  		       AIRCON_set_mode( AIRCON_mode_et mode );
AIRCON_mode_et     AIRCON_get_mode( void );
void  		       AIRCON_toggle_mode( void );
void               AIRCON_validate_temp_settings( void );
void 			   AIRCON_update_outputs( void );
void 			   AIRCON_set_heater_state( disable_enable_et state );
void 			   AIRCON_set_cooler_state( disable_enable_et state );
disable_enable_et  AIRCON_get_heater_state( void );
disable_enable_et  AIRCON_get_cooler_state( void );
disable_enable_et  AIRCON_get_state( void );
float			   AIRCON_get_temperature_setting( AIRCON_mode_et type );
void 			   AIRCON_adjust_temperature_setting(AIRCON_mode_et type, u8_t dir);
void 			   AIRCON_set_state( disable_enable_et state );
void 			   AIRCON_toggle_state( void );
void 			   AIRCON_handle_rotary_input( ROTARY_scroll_type_et type );
AIRCON_min_max_st* AIRCON_get_stats_address( void );

#endif /* AIRCON_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
