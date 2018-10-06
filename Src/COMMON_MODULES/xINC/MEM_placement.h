/*! \file
*               $Revision: 12657 $
*
*               $Author: amillen $
*
*               $Date: 2013-03-14 16:44:58 +0000 (Thu, 14 Mar 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/INC/trunk/MEM_placement.h $
*
*   \brief      Standard include file for mapping to desired memmap strategy
*
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
///*** NOTE: this file is intended to be multiply included!!

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "PROJ_config.h"

#if( MEM_PLACEMENT_AUTOSAR == 1 )
  #include "MemMap.h"
#elif( MEM_PLACEMENT_SEL == 1 )
  #include "MemMapSel.h"
#else
  /* Nothing to do */
#endif



/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None allowed */



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
/* None allowed */


/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None allowed */



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
/* None allowed */



/***************************************************************************************************
**                              Configuration Integrity Checks                                    **
***************************************************************************************************/
/* None */


/****************************** END OF FILE *******************************************************/
