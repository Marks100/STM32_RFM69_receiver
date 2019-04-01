/*! \file
*               $Revision: 21249 $
*
*               $Author: mstewart $
*
*               $Date: 2014-09-01 10:00:33 +0100 (Mon, 01 Sep 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/Flash_Write/flash_write.h $
*
*   \brief      Public header file for HAL_TIM module.
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
****************************************************************************************************/

#ifndef NVM_DEFAULTS_H
#define NVM_DEFAULTS_H

#include "PROJ_config.h"
#include "NVM.h"

/***************************************************************************************************
**                              Defines                                                          **
***************************************************************************************************/


/***************************************************************************************************
**                              Structure Types                                                  **
***************************************************************************************************/


/***************************************************************************************************
**                              Default NVM values                                                **
***************************************************************************************************/

const NVM_generic_data_blk_st      NVM_DEFAULT_GENERIC_DATA_BLK_s =
{
	RTC_EXT_DEFAULT_WAKEUP_TIME_SEC,						//Sleep time
	RFM69_DEFAULT_TX_POWER_LEVEL,				      		//Tx power level
	0x1234,	    											//Device node ID
	RFM69_433_DEFAULT_CONFIG,								//rf config
	NRF24_SETUP_RX,
	{ "help",
	  "ver",
	  "reset",
	  "nvm",
	  "",
	  "",
	}
};


#endif
