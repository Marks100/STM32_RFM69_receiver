/*! \file
*               $Revision: 210 $
*
*               $Author: IReid $
*
*               $Date: 2008-12-08 13:32:36 +0000 (Mon, 08 Dec 2008) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/TPMS/RF_DECODE/trunk/CRC.c $
*
*   \brief      CRC calculation module
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
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "CHKSUM.h"
#include "CHKSUM_pri.h"


/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/




/*!
****************************************************************************************************
*
*   \brief          Calculates standard byte-wise checksum of a buffer
*
*   \todo           Measure timing to benchmark against other checksum calcs
*
*   \author         EP
*
*   \param          buffer_p - pointer to buffer of data to calc checksum for
*   \param          buffer_length - length of buffer
*
*   \return         calculated checksum
*
***************************************************************************************************/
u8_t CHKSUM_calc_byte_wise_checksum( const u8_t* buffer_p, u8_t buffer_length )
{
    //! \note Something funny about this function and Cosmic, so this is checksum needs to be volatile
    volatile u8_t checksum;
    u8_t i;

    checksum = 0x00;

    for ( i=0; i < buffer_length; i++ )
    {
        checksum = (u8_t)(checksum + *(buffer_p + i));
    }
    return checksum;
}

/*!
****************************************************************************************************
*
*   \brief          Calculates checksum of a buffer "xoring" pars.
*
*   \todo           Measure timing to benchmark against other checksum calcs
*
*   \author         EP
*
*   \param          buffer_p - pointer to buffer of data to calc checksum for
*   \param          buffer_length - length of buffer
*
*   \return         calculated checksum
*
***************************************************************************************************/
u8_t CHKSUM_calc_xor_checksum( const u8_t* buffer_p, u8_t buffer_length )
{
    //! \note Something funny about this function and Cosmic, so this is checksum needs to be volatile
    volatile u8_t checksum;
    u8_t i;

    checksum = 0x00;

    for ( i=0; i < buffer_length; i++ )
    {
        checksum = (u8_t)( checksum ^ *(buffer_p + i) );
    }
    return checksum;
}








/****************************** END OF FILE *******************************************************/
