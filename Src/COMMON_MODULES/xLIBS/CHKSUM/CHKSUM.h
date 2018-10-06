/*! \file
*               $Revision: 185 $
*
*               $Author: IReid $
*
*               $Date: 2008-11-21 14:21:55 +0000 (Fri, 21 Nov 2008) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/TPMS/RF_DECODE/trunk/CRC.h $
*
*   \brief      Public header file for CRC calculation module
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
#ifndef CHKSUM_H
#define CHKSUM_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
// CRC-8     x^8  + x^2 + x + 1
#define POLY        0x0107
#define CRC_INIT    0x00


typedef enum CHKSUM_poly_type
{
    CHKSUM_POLY_0107       = 0u,
    CHKSUM_POLY_012F       = 1u,
    CHKSUM_POLY_MAX_VALUES = 2u
} CHKSUM_poly_type_et;

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

  u8_t  CHKSUM_calc_byte_wise_checksum( const u8_t* buffer_p, u8_t buffer_length );
  u8_t  CHKSUM_calc_xor_checksum( const u8_t* buffer_p, u8_t buffer_length );



#endif /* Multiple inclusion guard */
/****************************** END OF FILE *******************************************************/
