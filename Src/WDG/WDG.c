/*! \file
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*   \brief      WDG
*/

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "stm32f10x_iwdg.h"

#include "C_defs.h"
#include "STDC.h"


/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */

void WDG_init( void )
{
	/* This pauses the watchdog while debugging */
	DBGMCU->CR |= DBGMCU_CR_DBG_IWDG_STOP;

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_4); // 4, 8, 16 ... 256
	IWDG_SetReload(0x07D0);//This parameter must be a number between 0 and 0x0FFF.
	IWDG_ReloadCounter();
	IWDG_Enable();
}


void WDG_deinit( void )
{
	/* Cant disable :( */
}


void WDG_kick( void )
{
	IWDG_SetReload(0x07D0);//This parameter must be a number between 0 and 0x0FFF.
	IWDG_ReloadCounter();
}




///****************************** END OF FILE *******************************************************/