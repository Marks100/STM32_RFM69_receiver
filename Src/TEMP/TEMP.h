#ifndef TEMP_H
#define TEMP_H


/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#define TEMP_TICK_RATE_MS	50u
#define TEMP_TIMEOUT		( 60u * ( 1000u / TEMP_TICK_RATE_MS ) ) 



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
	TEMP_INIT = 0u,
	TEMP_SETUP,
	TEMP_RUNNING,
	TEMP_ERROR
	
} TEMP_state_et;




/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void          TEMP_init( void );
void          TEMP_cyclic( void );
s16_t         TEMP_get_temp( void );
TEMP_state_et TEMP_get_state( void );
void          TEMP_set_state( TEMP_state_et state );
pass_fail_et  TEMP_self_check( void );
void          TEMP_increment_fail_cnt( void );
float         TEMP_take_temp_measurement( void );
pass_fail_et  TEMP_get_last_temp_measurement( float* temp );
pass_fail_et  TEMP_validate_temp( float temp );


#endif /* TEMP_H multiple inclusion guard */

///****************************** END OF FILE *******************************************************/