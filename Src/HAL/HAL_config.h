#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"

#define ONBOARD_LED_PORT   GPIOA
#define ONBOARD_LED_PIN    GPIO_Pin_1

#define NRF24_CS_PORT      GPIOB
#define NRF24_CS_PIN       GPIO_Pin_2  

#define NRF24_CE_PORT      GPIOB
#define NRF24_CE_PIN       GPIO_Pin_0

#define SPI1_PORT          GPIOA
#define SPI1_SCK_PIN       GPIO_Pin_5
#define SPI1_MISO_PIN      GPIO_Pin_6
#define SPI1_MOSI_PIN      GPIO_Pin_7

#define SPI2_PORT          GPIOB
#define SPI2_SCK_PIN       GPIO_Pin_3
#define SPI2_MISO_PIN      GPIO_Pin_4
#define SPI2_MOSI_PIN      GPIO_Pin_5

#define USART2_PORT        GPIOA
#define USART2_TX_PIN      GPIO_Pin_2
#define USART2_RX_PIN      GPIO_Pin_3

#define I2C1_PORT          GPIOB
#define I2C1_SDA_PIN       GPIO_Pin_7
#define I2C1_SCL_PIN       GPIO_Pin_6

#define ONBOARD_BTN_PORT   GPIOA
#define ONBOARD_BTN_PIN    GPIO_Pin_8

#define ROTARY_PORT        GPIOA
#define ROTARY_CLK_PIN     GPIO_Pin_12
#define ROTARY_DATA_PIN    GPIO_Pin_11
#define ROTARY_SW_PIN      GPIO_Pin_8
#define ROTARY_PORT_SOURCE GPIO_PortSourceGPIOA
#define ROTARY_PIN_SOURCE  GPIO_PinSource11
#define ROTARY_EXT_LINE    EXTI_Line11

#define SHIFT_REG_PORT     GPIOA
#define SHIFT_REG_CLK_PIN  GPIO_Pin_11
#define SHIFT_REG_DATA_PIN GPIO_Pin_12   

#define LCD_PORT          GPIOA
#define LCD_RS_PIN        GPIO_Pin_13
#define LCD_EN_PIN        GPIO_Pin_14




#endif /* HAL_CONFIG_H multiple inclusion guard */
