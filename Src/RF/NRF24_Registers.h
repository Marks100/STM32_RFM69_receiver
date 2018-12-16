/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/HAL/RF/RF.h $
*
*   \brief      Public header file for RF module.
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
*         (C) $Date:: 2014#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/
#ifndef NRF24_REGISTERS_H
#define NRF24_REGISTERS_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "NRF24.h"



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/


/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/



/* Default register set 433Mhz ( 19200bps ) */
const NRF24_register_data_st NRF24_default_register_set[] =
{
    { CONFIG,       0x0E},
	{ EN_AUTO_ACK,  0x00},
	{ EN_RXADDR,    0x03},
    { SETUP_AW,     0x00},
	{ SETUP_RETR,   0x00},
	{ RF_CH,        0x6C},
	{ RF_SETUP,     0x26},
	{ STATUS,       0x00},
	{ OBSERVE_TX,   0x00},
	{ CD,           0x00},
	{ RX_ADDR_P0,   0x00},
	{ RX_ADDR_P1,   0x00},
	{ RX_ADDR_P2,   0x00},
	{ RX_ADDR_P3,   0x00},
	{ RX_ADDR_P4,   0x00},
	{ RX_ADDR_P5,   0x00},
	{ TX_ADDR ,     0x00},
	{ RX_PW_P0,     0x20},
	{ RX_PW_P1,     0x00},
	{ RX_PW_P2,     0x00},
	{ RX_PW_P3,     0x00},
	{ RX_PW_P4,     0x00},
	{ RX_PW_P5,     0x00},
	{ FIFO_STATUS,  0x00},
	{ DYNPD,        0x00},
	{ FEATURE,      0x00},
    { ADDRESS_NOP,  0x00},
};



#define DEFAULT_CONFIGURATION_SIZE      ( 27u )

const NRF24_static_configuration_st NRF24_config_c[ 1 ] =
{
    { (NRF24_register_data_st * )NRF24_default_register_set, DEFAULT_CONFIGURATION_SIZE },
};



#endif /* NRF24_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
