#ifndef HAL_BRD_H
#define HAL_BRD_H



/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
/* None */
#define SELECTOR_MODE_BIT_MASK ( 0x0F )


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None */

typedef enum
{
    SLIDER_1 = 0u,
    SLIDER_2,
    SLIDER_3,
    SLIDER_4
} HAL_BRD_switch_slider_et;



/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */
extern u8_t debug_mode;


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/

void HAL_BRD_init( void );
low_high_et HAL_BRD_Read_Pin_state( GPIO_TypeDef * port, u16_t pin );
void HAL_BRD_Set_Pin_state(  GPIO_TypeDef * port, u16_t pin, low_high_et state );

void HAL_BRD_Set_batt_monitor_state( disable_enable_et state );
void HAL_BRD_Set_rf_enable_pin( disable_enable_et state );
void HAL_BRD_Toggle_led( void );
void HAL_BRD_set_LED( off_on_et state );
void HAL_BRD_RFM69_set_enable_Pin_state( low_high_et state );
void HAL_BRD_RFM69_set_reset_Pin_state( low_high_et state );
void HAL_BRD_RFM69_spi_slave_select( low_high_et state );
low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider );

false_true_et HAL_BRD_get_rtc_trigger_status( void );
void HAL_BRD_set_rtc_trigger_status( false_true_et state );
disable_enable_et HAL_BRD_read_debug_pin( void );



#endif
