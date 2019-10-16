#ifndef HAL_BRD_H
#define HAL_BRD_H



/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"

#if(UNIT_TEST == 0)
    #include "stm32f10x_gpio.h"
    #include "stm32f10x_rcc.h"
#else
    #include "STM32_stubs.h"
#endif


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
    SLIDER_4,
    SLIDER_MAX
} HAL_BRD_switch_slider_et;


typedef enum
{
	LED_0 = 0,
	LED_1,
	LED_2,
	LED_3,
	LED_MAX
} HAL_BRD_led_et;


/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/

void              HAL_BRD_init( void );
void              HAL_BRD_reset( void );
low_high_et       HAL_BRD_read_pin_state( GPIO_TypeDef * port, u16_t pin );
void              HAL_BRD_set_pin_state(  GPIO_TypeDef * port, u16_t pin, low_high_et state );
void              HAL_BRD_toggle_onboard_led( void );
low_high_et       HAL_BRD_read_Onboard_btn_state( void );

void              HAL_BRD_set_batt_monitor_state( disable_enable_et state );
void              HAL_BRD_set_rf_enable_pin( disable_enable_et state );
void              HAL_BRD_set_LED( off_on_et state );
low_high_et       HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider );
void              HAL_BRD_NRF24_spi_slave_select( low_high_et state );
void              HAL_BRD_NRF24_set_ce_pin_state( low_high_et state );
void              HAL_BRD_set_LED_state( HAL_BRD_led_et led, low_high_et state );
void              HAL_BRD_set_onboard_LED( off_on_et state );
void              HAL_BRD_set_ROTARY_interrupt_state( disable_enable_et state );
void              HAL_BRD_debounce_completed( void );
void              HAL_BRD_set_generic_output( u8_t generic_output_num, off_on_et state );
false_true_et     HAL_BRD_get_rtc_trigger_status( void );
void              HAL_BRD_set_rtc_trigger_status( false_true_et state );
disable_enable_et HAL_BRD_read_debug_pin( void );
void              HAL_BRD_set_cooler_state( off_on_et state );
void              HAL_BRD_set_heater_state( off_on_et state );
low_high_et       HAL_BRD_read_rotary_clock_pin( void );
low_high_et       HAL_BRD_read_rotary_data_pin( void );
low_high_et       HAL_BRD_read_rotary_btn_pin( void );

void              HAL_BRD_74HC164_set_clk_pin_state( low_high_et state );
void              HAL_BRD_74HC164_pulse_clk_pin_state( void );
void              HAL_BRD_74HC164_set_data_pin_high( void );
void              HAL_BRD_74HC164_set_data_pin_low( void );
void              HAL_BRD_LCD_pulse_enable_pin_state( void );
void              HAL_BRD_LCD_set_enable_pin_low( void );
void              HAL_BRD_LCD_set_enable_pin_high( void );
void              HAL_BRD_LCD_set_RS_pin_high( void );
void              HAL_BRD_LCD_set_RS_pin_low( void );
void              HAL_BRD_get_SW_version_num( u8_t *version_num_p );
void              HAL_BRD_get_HW_version_num( u8_t *version_num_p );

void ROTARY_evaluate_signals( low_high_et clock, low_high_et data );
void ROTARY_tick( void );

#endif
