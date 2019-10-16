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


/***************************************************************************************************
**                              Defines                                                          **
***************************************************************************************************/


/***************************************************************************************************
**                              Structure Types                                                  **
***************************************************************************************************/


/***************************************************************************************************
**                              Default NVM values                                                **
***************************************************************************************************/

const NVM_generic_data_blk_st NVM_DEFAULT_GENERIC_DATA_BLK_s =
{
	RTC_EXT_DEFAULT_WAKEUP_TIME_SEC,						//Sleep time
	0x1234,	    											//Device ID
	NRF24_SETUP_RX,											//rf manager mode
	{
		{ "help" },											//default CLI commands
		{ "ver" },
		{ "reset" },
		{ "nvm" },
		{ "" },
		{ "" },
	},
	{
		0x0001, 0x0002, 0x0003, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, //Whitelist
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
	},
	DISABLE_,   											//Whitelist state
	DISABLE_,												//RF debug output state
	AIRCON_COOL_MODE,
	DISABLE_,
	15.5f,
	19.5f,
	18.0f,
};


#endif
