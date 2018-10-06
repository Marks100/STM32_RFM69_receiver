/*! \file
*               $Revision: 14979 $
*
*               $Author: ireid $
*
*               $Date: 2013-09-16 08:24:38 +0100 (Mon, 16 Sep 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/INC/trunk/RTOS.h $
*
*   \brief      SEL RTOS redirection header File
*/
/* COPYRIGHT NOTICE
* =================
*
* The contents of this document are protected under copyright and contain
* commercially and/or technically confidential information. The content of
* this document must not be used other than for the purpose for which it was
* provided nor may it be disclosed or copied (by the authorised recipient or
* otherwise) without the prior written consent of an authorised officer of
* Schrader Electronics Ltd.
*
*         (C) $Date:: 2013#$ Schrader Electronics Ltd.
*
*******************************************************************************
*/
#ifndef RTOS_H
#define RTOS_H


/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "PROJ_config.h"
#include "COMPILER_config.h"

#if( PRO_OSEK_RTOS )
  #include "os.h"

#elif( OSEK_RTOS )
  #include "osek.h"
  #include "osekext.h"  // Have to include this here as Vector only include it in *.c so it doesn't get picked up

#elif( FREE_RTOS )
  #include "FreeRTOS.h"
  #include "task.h"
  #include "queue.h"
  #include "semphr.h"

#elif( SEL_RTOS )
  #include "SEL_RTOS.h"

#elif( NO_RTOS )
  /* No includes */

#else
  #error "Please speicfy an RTOS type for this project in PROJ_config.h"

#endif // #if( PRO_OSEK_RTOS )

// Fn like macro being used for optimisation
/* PRQA S 3429,3453 EOF */

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#if( PRO_OSEK_RTOS )
  // remap SEL's RTOS_schedule_task() function to OSEK's ActivateTask() function
  #define RTOS_schedule_task(task)    ActivateTask(task)

#elif( OSEK_RTOS )
  // remap SEL's RTOS_schedule_task() function to OSEK's ActivateTask() function
  #define RTOS_schedule_task(task)    ActivateTask(task)

#elif( FREE_RTOS )
  #define DeclareTask(fn)             extern void fn(void)								/* PRQA S 3410 */ /* This is the intended behaviour */
  #define TASK(fn)                    void fn(void)										/* PRQA S 3410 */ /* This is the intended behaviour */
  #define TerminateTask()             (0)
  #define DeclareAlarm(fn)            extern void fn(void)
  #define RTOS_schedule_task(task)    RTOS_APP_Activate_##task()

#elif( SEL_RTOS )
  #define DeclareTask(fn)             extern FUNC(void, SEL_RTOS_TASK_CODE) fn(void)    /* PRQA S 3410 */ /* This is the intended behaviour */
  #define TASK(fn)                    FUNC(void, SEL_RTOS_TASK_CODE) fn(void)           /* PRQA S 3410 */ /* This is the intended behaviour */
  #define TerminateTask()             (0)
  #define DeclareAlarm(fn)            (0)
  #define RTOS_schedule_task(task)    SEL_RTOS_schedule_task( task )

#elif( NO_RTOS )
  #define DeclareTask(fn)             extern void fn(void)								/* PRQA S 3410 */ /* This is the intended behaviour */
  #define TASK(fn)                    void fn(void)										/* PRQA S 3410 */ /* This is the intended behaviour */
  #define TerminateTask()             (0)
  #define DeclareAlarm(fn)            extern void fn(void)								/* PRQA S 3410 */ /* This is the intended behaviour */
  #define RTOS_schedule_task(task)    task()

#endif


#endif /* RTOS_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
