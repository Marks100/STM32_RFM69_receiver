/*! \file
*               $Revision: 12850 $
*
*               $Author: amillen $
*
*               $Date: 2013-04-11 11:40:00 +0100 (Thu, 11 Apr 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/RTOS/SEL_RTOS/trunk/Templates/SEL_RTOS_config_template.h $
*
*   \brief      Module configuration file for SEL RTOS.
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
#ifndef SEL_RTOS_CONFIG_H
#define SEL_RTOS_CONFIG_H
/** ------------------------------------------------------------------------------------------------
---                            PUBLIC Configuration Section                                      ---
--------------------------------------------------------------------------------------------------*/
/* This section should be used for anything that may be required by another module, including
version information, feature configuration, etc. */


/***************************************************************************************************
**                              Version Information                                               **
***************************************************************************************************/
/* config file version */
#define SEL_RTOS_CONFIG_VERSION          ( 7u )

/* Expected Common Module SVN revision */
#define SEL_RTOS_EXPECTED_SVN_REVISION   ( #error "Please enter expected SVN revision here." )



/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/



/***************************************************************************************************
**                              Defines, etc.                                                     **
***************************************************************************************************/
/* Features */
#define SEL_RTOS_TIMED_TASKS_ENABLE   ( 0 )    // Whether or not to output task timing information
#define SEL_RTOS_MONITOR_TASK_LENGTH  ( 1 )    // Whether or not to error when task is over time

// Enable if tasks are to be services from within tick context.
#define RTOS_CFG_SERVICE_TASKS_WITHIN_TICK	( 1 )

/* For optimizing the RTOS ( a little ), disable extended Timers */
#define SEL_RTOS_EXTENDED_TIMER_RANGE ( 0 )    // Enables  1 sec, 1 minut and 1 hour timers


/***************************************************************************************************
**                              Public Data Types and Enums                                       **
***************************************************************************************************/
// List of tasks - lower the SEL_RTOS_taskname_et enum the higher the task priority
typedef enum
{
    SEL_RTOS_WDG_TASK,
    SEL_RTOS_RF_TASK,
    SEL_RTOS_JOYSTICK_TASK,
    SEL_RTOS_LCD_API_TASK,
    SEL_RTOS_BUTTON_TASK,
    SEL_RTOS_NVM_TASK,
    SEL_RTOS_MODE_MGR_TASK,
    SEL_RTOS_LED_TASK,
    SEL_RTOS_STARTUP_TASK,
    SEL_RTOS_TASK_MAX

}SEL_RTOS_taskname_et;


// List of Timers that drive the cyclic tasks
typedef enum
{
    SEL_RTOS_WDG_TIMER,
    SEL_RTOS_RF_TMER,
    SEL_RTOS_JOYSTICK_TMER,
    SEL_RTOS_LCD_API_TIMER,
    SEL_RTOS_BUTTON_TIMER,
    SEL_RTOS_NVM_TIMER,
    SEL_RTOS_MODE_MGR_TIMER,
    SEL_RTOS_LED_TIMER,
    SEL_RTOS_STARTUP_TIMER,
    SEL_RTOS_TIMER_MAX,

}SEL_RTOS_timername_et;



#endif /* SEL_RTOS_CONFIG_H multiple inclusion guard */








/* Header guard to make this section only include-able via private header file */
#if( defined( SEL_RTOS_PRI_H ) && !defined( SEL_RTOS_PRIVATE_CONFIG_H ) )
#define SEL_RTOS_PRIVATE_CONFIG_H
/** ------------------------------------------------------------------------------------------------
---                            PRIVATE Configuration Section                                     ---
--------------------------------------------------------------------------------------------------*/
/* This section should be used for anything that is required internally to make the module work.
Anything that an outside module would never need to access should be placed in this section. */
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "SEL_RTOS_APP_pub.h"  /* For redirections */



/***************************************************************************************************
**                              Private Data Types and Enums                                      **
***************************************************************************************************/
//e.g. typedef enum enumeration_a .....



/***************************************************************************************************
**                              Defines, etc.                                                     **
***************************************************************************************************/
/* Redirections */
#define SEL_RTOS_CFG_startup_hook()                  SEL_RTOS_APP_startup_hook()
#define SEL_RTOS_CFG_shutdown_hook()                 SEL_RTOS_APP_shutdown_hook()
#define SEL_RTOS_CFG_os_error(task, error)           SEL_RTOS_APP_os_error(task, error)
#define SEL_RTOS_CFG_get_16bit_reference_time()      SEL_RTOS_APP_get_16bit_reference_time()
#define SEL_RTOS_CFG_pre_task_hook( task_name )      SEL_RTOS_APP_pre_task_call( task_name )
#define SEL_RTOS_CFG_post_task_hook( task_name )     SEL_RTOS_APP_post_task_call( task_name )
#define SEL_RTOS_CFG_idle_hook()                     SEL_RTOS_APP_idle_hook()
#define SEL_RTOS_CFG_log_16_t_task_time( time )      SEL_RTOS_APP_send_16_t()
#define SEL_RTOS_CFG_enter_critical_section()        //HAL_INT_disable_global_interrupts() 		// insert your own Global Interupt Disable function here
#define SEL_RTOS_CFG_leave_critical_section()        //HAL_INT_enable_global_interrupts()			// insert your own Global Interupt Enable function here

/***************************************************************************************************
**                              TASK declarations, etc                                            **
***************************************************************************************************/
/* Constant Array of taskname and reload cnt */
const task_timer_st timer_array[ NUMBER_OF_CYCLIC_TASKS ] =
{
    /* Task to be scheduled    Timer resolution   Cyclic Time   Offset  Autostart */
    { SEL_RTOS_WDG_TASK,         TIMER_10MS_RES,  10u,          10,     SEL_TRUE      },
    { SEL_RTOS_RF_TASK,          TIMER_10MS_RES,  2u,           10,     SEL_TRUE      },
    { SEL_RTOS_JOYSTICK_TASK,    TIMER_10MS_RES,  2u,           10,     SEL_TRUE      },
    { SEL_RTOS_LCD_API_TASK,     TIMER_10MS_RES,  20u,          20,     SEL_TRUE      },
    { SEL_RTOS_BUTTON_TASK,      TIMER_10MS_RES,  1u,           30,     SEL_TRUE      },
    { SEL_RTOS_NVM_TASK,         TIMER_250MS_RES, 4u,           10,     SEL_TRUE      },
    { SEL_RTOS_MODE_MGR_TASK,    TIMER_10MS_RES,  1u,           40,     SEL_TRUE      },
    { SEL_RTOS_LED_TASK,         TIMER_10MS_RES,  20u,          10,     SEL_TRUE      },
    { SEL_RTOS_STARTUP_TASK,     TIMER_10MS_RES,  10u,          0,      SEL_TRUE      }
    };

/* Task declarations */
DeclareTask( rtos_task_wdg_tick );
DeclareTask( rtos_task_rf_tick );
DeclareTask( rtos_task_joystick_tick );
DeclareTask( rtos_task_lcd_api_tick );
DeclareTask( rtos_task_button_tick );
DeclareTask( rtos_task_nvm_tick );
DeclareTask( rtos_task_mode_mgr_tick );
DeclareTask( rtos_task_led_tick );
DeclareTask( rtos_task_startup_tick );

/* Array of task information */
const task_st task_array[ NUMBER_OF_TASKS ] =
{
    /* Function that implements this task    Max run time(ms) */
    { rtos_task_wdg_tick,        30u },
    { rtos_task_rf_tick,         20u },
    { rtos_task_joystick_tick,   20u },
    { rtos_task_lcd_api_tick,    30u },
    { rtos_task_button_tick,     20u },
    { rtos_task_nvm_tick,        20u },
    { rtos_task_mode_mgr_tick,   20u },
    { rtos_task_led_tick,        20u },
    { rtos_task_startup_tick,    20u }
};


#endif /* SEL_RTOS_PRIVATE_CONFIG_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/

