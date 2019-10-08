#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"

#define ONBOARD_LED_PORT GPIOA
#define ONBOARD_LED_PIN  GPIO_Pin_1

#define NRF24_CS_PORT    GPIOB
#define NRF24_CS_PIN     GPIO_Pin_2  

#define NRF24_CE_PORT    GPIOB
#define NRF24_CE_PIN     GPIO_Pin_0  


#endif /* HAL_CONFIG_H multiple inclusion guard */
