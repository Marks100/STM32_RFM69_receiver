
#ifndef HAL_I2C_H
#define HAL_I2C_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#define RTC_EXT_MAX_WAKEUP_TIME_SEC 	15300
#define RTC_EXT_I2C_ADDRESS				0xA2
#define RTC_EXT_MAX_NUM_REGS			16


#define RTC_EXT_TIMER_INT_ENABLE_BIT (1<<0)
#define RTC_EXT_ALARM_INT_ENABLE_BIT (1<<1)
#define RTC_EXT_TIMER_INT_ACTIVE_BIT (1<<2)
#define RTC_EXT_ALARM_INT_ACTIVE_BIT (1<<3)

#define RTC_EXT_ALARM_1HZ_BIT (1<<1)
#define RTC_EXT_ALARM_1_OVER60HZ_BIT (1<<0)

typedef enum
{
	Control_status_1 = 0u,
	Control_status_2,
	VL_seconds,
	Minutes,
	Hours,
	Days,
	Weekdays,
	Century_months,
	Years,
	Minute_alarm,
	Hour_alarm,
	Day_alarm,
	Weekday_alarm,
	CLKOUT_control,
	Timer_control,
	Timer
} RTC_EXT_registers_et;



/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


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
void HAL_I2C_init( void );
void HAL_I2C_de_init( void );

void HAL_I2C_write_single_register( u8_t dev_address, u8_t register_address, u8_t* data );
void HAL_I2C_write_multiple_register( u8_t dev_address, u8_t register_start_address, u8_t* data, u8_t num_bytes );
void HAL_I2C_read_register(  u8_t dev_add, u8_t reg_add, u8_t* data );
void HAL_I2C_read_multiple_registers( u8_t dev_add, u8_t reg_start_add, u8_t* data, u8_t num_bytes );

void RTC_ext_init( void );
void RTC_set_wakeup_time( u32_t seconds );




#endif /* HAL_UART_PUB_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
