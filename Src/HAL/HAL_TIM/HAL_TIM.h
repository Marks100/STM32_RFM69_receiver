#ifndef HAL_TIM_H
#define HAL_TIM_H


/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void HAL_TIM1_init( void );
void HAL_TIM2_init( void );
void HAL_TIM1_start( void );
void HAL_TIM1_stop( void );
void HAL_TIM2_stop( void );
void HAL_TIM2_start( void );

void HAL_TIM3_stop( void );
void HAL_TIM3_start( void );

void  HAL_TIM_increment_secs( void );
u32_t HAL_TIM_get_time( void );

void HAL_TIM_3_SetAutoreload( u16_t new_time_ms );









#endif
