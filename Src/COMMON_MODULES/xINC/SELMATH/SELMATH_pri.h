/*! \file
*               $Revision: 13824 $
*
*               $Author: ireid $
*
*               $Date: 2013-06-24 08:30:20 +0100 (Mon, 24 Jun 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/LIBS/trunk/SELMATH/SELMATH_pri.h $
*
*   \brief      private header file for the Schrader math module
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
#ifndef SELMATH_PRI_H
#define SELMATH_PRI_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
//! ANSI C / GNU LCG multiplier and increment
#define LCG_MULTIPLIER  ( 1103515245 )
#define LCG_INCREMENT   ( 12345 )

#define SELMATH_GENERATE_SEED_MAX_ATTEMPTS      ( 3u )
#define SELMATH_GENERATE_SEED_NUM_LCG_STEPS     ( 13u )
#define INVALID_SEED_MAX                        ( U32_T_MAX )

//! Default atmospheric pressure in mbar
#define DEFAULT_ATMOSPHERIC_PRESSURE_MBAR (mbar_u16_t)( 1013u )


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
#define SELMATH_START_SEC_CONST_8
#include "MEM_placement.h"
  /* lookup table for STDC_sqrt() */
  static const u8_t square_root_table_s[ 8 ] =
  {
      4u,  //   0 -  31
      6u,  //  32 -  63
      8u,  //  64 -  95
      10u, //  96 - 127
      12u, // 128 - 159
      13u, // 160 - 191
      14u, // 192 - 223
      16u  // 224 - 255
  };
#define SELMATH_STOP_SEC_CONST_8
#include "MEM_placement.h"



/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Configuration Integrity Checks                                    **
***************************************************************************************************/
/* None */


#endif /* Multiple inclusion guard */
/****************************** END OF FILE *******************************************************/
