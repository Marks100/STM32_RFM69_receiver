/*! \file
*               $Revision: 14980 $
*
*               $Author: ireid $
*
*               $Date: 2013-09-16 08:32:32 +0100 (Mon, 16 Sep 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/INC/trunk/TPMS_types.h $
*
*   \brief      Standard include file for TPMS
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
#ifndef TPMS_TYPES_H
#define TPMS_TYPES_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "COMPILER_defs.h"

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
/* None */

/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/

//! \note TPMS_types.h typedef naming convention, Agreed by Ivan and Alan, 18-June-2013
//! \note <unit>_<originating type>_t
//! \note e.g degrees_c_s16_t, mbar_u16_t, secs_u8_t
//! \note An invalid value should be defined for each type to be used if the information being passed is invalid

//! \position map for use in various SEL modules for identifying wheel position
typedef struct pos_map
{
    u32_t id;       //! TPMS Wheel Unit Id
    u8_t  g_idx;    //! TPMS WHeel Unit Global Id

} pos_map_st;

// Temperatures
typedef s8_t   degrees_c_s8_t;                  //! temperature in degrees Celsius (degC), -127 to +128 degC
typedef s16_t  degrees_c_s16_t;                 //! temperature in degrees Celsius (degC), -32,767 to 32,768 degC
typedef u16_t  degrees_k_u16_t;                 //! temperature in Kelvin (K), 0 to 65,535 Kelvin

#define DEGREES_C_S8_INVALID                    ( ( degrees_c_s8_t  )( S8_T_MAX ) )  //! Indicates that value is invalid
#define DEGREES_C_S16_INVALID                   ( ( degrees_c_s16_t )( S16_T_MAX ) ) //! Indicates that value is invalid
#define DEGREES_K_U16_INVALID                   ( ( degrees_k_u16_t )( U16_T_MAX ) ) //! Indicates that value is invalid

// Pressures
typedef u16_t   mbar_u16_t;                     //! pressure in millibar (mbar), 0 to 65,536 mbar
typedef u32_t   mbar_u32_t;                     //! pressure in millibar (mbar), 0 to 4,294,967,296 mbar

#define MBAR_U16_INVALID                        ( ( mbar_u16_t )( U16_T_MAX ) )  //! Indicates that value is invalid
#define MBAR_U32_INVALID                        ( ( mbar_u32_t )( U32_T_MAX ) )  //! Indicates that value is invalid

// Distances
typedef u8_t    km_u8_t;                        //! 8bit unsigned distance in km  0 to 256 km type
typedef u16_t   km_u16_t;                       //! 16bit unsigned distance in km  0 to 65,536 km type
typedef u32_t   km_u32_t;                       //! 32bit unsigned distance in km  0 to 4,294,967,296 km type.  E.g. Could be used for odometer values

#define KM_U8_INVALID                           ( ( km_u8_t )( U8_T_MAX ) )     //! Indicates that value is invalid
#define KM_U16_INVALID                          ( ( km_u16_t )( U16_T_MAX ) )   //! Indicates that value is invalid
#define KM_U32_INVALID                          ( ( km_u32_t )( U32_T_MAX ) )   //! Indicates that value is invalid

// Time
typedef u8_t    ten_msecs_u8_t;                 //! 8bit unsigned time in units of 10msecs  0 to 2550 msecs type.
typedef u16_t   ten_msecs_u16_t;                //! 16bit unsigned time in units of 10msecs  0 to 655,360 msecs type.
typedef u32_t   ten_msecs_u32_t;                //! 32bit unsigned time in units of 10msecs  0 to 42,949,672,960 msecs type.

#define TEN_MSECS_U8_INVALID                    (ten_msecs_u8_t)( U8_T_MAX )   //! Indicates that value is invalid
#define TEN_MSECS_U16_INVALID                   (ten_msecs_u16_t)( U16_T_MAX ) //! Indicates that value is invalid
#define TEN_MSECS_U32_INVALID                   (ten_msecs_u32_t)( U32_T_MAX ) //! Indicates that value is invalid

// Time
typedef u8_t    hundred_msecs_u8_t;             //! 8bit unsigned time in units of 100msecs  0 to 25,500 msecs type.
typedef u16_t   hundred_msecs_u16_t;            //! 16bit unsigned time in units of 100msecs  0 to 6,553,600 msecs type.
typedef u32_t   hundred_msecs_u32_t;            //! 32bit unsigned time in units of 100msecs  0 to 429,496,729,600 msecs type.

#define HUNDRED_MSECS_U8_INVALID                (hundred_msecs_u8_t)( U8_T_MAX )   //! Indicates that value is invalid
#define HUNDRED_MSECS_U16_INVALID               (hundred_msecs_u16_t)( U16_T_MAX ) //! Indicates that value is invalid
#define HUNDRED_MSECS_U32_INVALID               (hundred_msecs_u32_t)( U32_T_MAX ) //! Indicates that value is invalid

typedef u8_t    secs_u8_t;                      //! 8bit unsigned time in secs  0 to 255 secs type.
typedef u16_t   secs_u16_t;                     //! 16bit unsigned time in secs  0 to 65,536 secs type.
typedef u32_t   secs_u32_t;                     //! 32bit unsigned time in secs  0 to 4,294,967,296 secs type.

#define SECS_U8_INVALID                         ( ( secs_u8_t )( U8_T_MAX ) )       //! Indicates that value is invalid
#define SECS_U16_INVALID                        ( ( secs_u16_t )( U16_T_MAX ) )     //! Indicates that value is invalid
#define SECS_U32_INVALID                        ( ( secs_u32_t )( U32_T_MAX ) )     //! Indicates that value is invalid

// Speeds
typedef u8_t    kph_u8_t;                       //! 8bit unsigned speed in km per hour 0 to 255 kph type.
typedef u16_t   kph_u16_t;                      //! 16bit unsigned speed in km per hour 0 to 65,536 kph type.

#define KPH_U8_INVALID                          ( ( kph_u8_t )( U8_T_MAX ) )     //! Indicates that value is invalid
#define KPH_U16_INVALID                         ( ( kph_u16_t )( U16_T_MAX ) )   //! Indicates that value is invalid

/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None allowed */

/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
/* None allowed */

#endif /* Multiple inclusion guard */
/****************************** END OF FILE *******************************************************/
