/*! \file
*               $Revision: 16060 $
*
*               $Author: ireid $
*
*               $Date: 2013-11-15 12:49:16 +0000 (Fri, 15 Nov 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/LIBS/trunk/SELMATH/SELMATH.h $
*
*   \brief      Public header file for the Schrader math module
*/
/* COPYRIGHT NOTICE
* ==================================================================================================
*
* The contents of this document are protected under copyright and contain commercially and/or
* technically confidential information. The content of this document must not be used other than
* for the purpose for which it was provided nor may it be disclosed or copied (by the authorised
* recipient or otherwise) without the prior written consent of an authorised officer of Schrader
* Electronics Ltd.
*
*         (C) $Date:: 2013#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/
#ifndef SELMATH_H
#define SELMATH_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "TPMS_types.h"

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#define SELMATH_SEED_SIZE                       ( 8u )
#define SELMATH_NUM_INDIVIDUAL_SEEDS            ( SELMATH_SEED_SIZE / sizeof( SELMATH_seed_t ) )


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
//! Typedef the size of seed & key
typedef u32_t SELMATH_seed_t;
typedef u32_t SELMATH_key_t;


/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
#define SELMATH_START_SEC_CODE
#include "MEM_placement.h"
  u8_t       		SELMATH_divide_u8_with_rounding( u8_t dividend, u8_t divisor );
  u16_t      		SELMATH_divide_u16_with_rounding( u16_t dividend, u16_t divisor );
  u32_t      		SELMATH_divide_u32_with_rounding( u32_t dividend, u32_t divisor );
  u8_t       		SELMATH_sqrt( u8_t number_to_square_root );
  u8_t       		SELMATH_abs(u8_t x, u8_t y);
  u16_t      		SELMATH_abs_word(u16_t x, u16_t y);
  u32_t      		SELMATH_abs_long( u32_t x, u32_t y );
  u8_t       		SELMATH_max_u8_t(u8_t x, u8_t y);
  u16_t      		SELMATH_max_u16_t( u16_t x, u16_t y );
  u32_t      		SELMATH_max_u32_t( u32_t x, u32_t y );
  u8_t       		SELMATH_min_u8_t(u8_t x, u8_t y);
  u16_t      		SELMATH_min_u16_t(u16_t x, u16_t y);
  u32_t      		SELMATH_min_u32_t(u32_t x, u32_t y);
  u32_t      		SELMATH_lcg( u32_t current_value );
  void       		SELMATH_generate_seed( SELMATH_seed_t initial_seeds[SELMATH_NUM_INDIVIDUAL_SEEDS], SELMATH_seed_t generated_seeds[SELMATH_NUM_INDIVIDUAL_SEEDS] );
  SELMATH_key_t 	SELMATH_calculate_key( SELMATH_seed_t seeds[SELMATH_NUM_INDIVIDUAL_SEEDS], u32_t constant );
  mbar_u16_t 		SELMATH_calculate_tc_pressure( mbar_u16_t raw_pressure, degrees_c_s16_t raw_temperature, degrees_c_s16_t ref_temperature );
  mbar_u16_t 		SELMATH_apply_atmospheric_compensation( mbar_u16_t pressure, mbar_u16_t atmospheric_pressure );
  u8_t              SELMATH_calculate_percent( u16_t para1, u32_t para2 );
  u8_t              SELMATH_1st_order_u8_filter( u8_t last_filtered_value, u8_t new_value, u8_t new_value_weighting );
  u16_t             SELMATH_1st_order_u16_filter( u16_t last_filtered_value, u16_t new_value, u8_t new_value_weighting );
  u32_t             SELMATH_map_ranges( u32_t value, u32_t input_min, u32_t input_max, u32_t output_min, u32_t output_max );
#define SELMATH_STOP_SEC_CODE
#include "MEM_placement.h"


#endif /* Multiple inclusion guard */
/****************************** END OF FILE *******************************************************/
