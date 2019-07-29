//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "STDC.h"
#include "CHKSUM.h"
#include "NVM.h"

#include "STM32_stubs.h"


//-- module being tested
#include "HAL_I2C.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/



/*******************************************************************************
 *     Variables
 ******************************************************************************/
NVM_info_st NVM_info_s;

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void)
{
}

void tearDown(void)
{
}

/*******************************************************************************
 *    TESTS
 ******************************************************************************/

void test_init(void)
{
	HAL_I2C_init();
}



  
void I2C_DeInit(u8_t* I2Cx){}
void I2C_Init(u8_t* I2Cx, u8_t* I2C_InitStruct){}
void I2C_StructInit(u8_t* I2C_InitStruct){}
void I2C_Cmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_DMACmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_DMALastTransferCmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_GenerateSTART(u8_t* I2Cx, false_true_et NewState){}
void I2C_GenerateSTOP(u8_t* I2Cx, false_true_et NewState){}
void I2C_AcknowledgeConfig(u8_t* I2Cx, false_true_et NewState){}
void I2C_OwnAddress2Config(u8_t* I2Cx, u8_t Address){}
void I2C_DualAddressCmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_GeneralCallCmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_ITConfig(u8_t* I2Cx, u16_t I2C_IT, false_true_et NewState){}
void I2C_SendData(u8_t* I2Cx, u8_t Data){}
u8_t I2C_ReceiveData(u8_t* I2Cx){}
void I2C_Send7bitAddress(u8_t* I2Cx, u8_t Address, u8_t I2C_Direction){}
u16_t I2C_ReadRegister(u8_t* I2Cx, u8_t I2C_Register){}
void I2C_SoftwareResetCmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_NACKPositionConfig(u8_t* I2Cx, u16_t I2C_NACKPosition){}
void I2C_SMBusAlertConfig(u8_t* I2Cx, u16_t I2C_SMBusAlert){}
void I2C_TransmitPEC(u8_t* I2Cx, false_true_et NewState){}
void I2C_PECPositionConfig(u8_t* I2Cx, u16_t I2C_PECPosition){}
void I2C_CalculatePEC(u8_t* I2Cx, false_true_et NewState){}
u8_t I2C_GetPEC(u8_t* I2Cx){}
void I2C_ARPCmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_StretchClockCmd(u8_t* I2Cx, false_true_et NewState){}
void I2C_FastModeDutyCycleConfig(u8_t* I2Cx, u16_t I2C_DutyCycle){}
u8_t I2C_CheckEvent(u8_t* I2Cx, u32_t I2C_EVENT){}

void RCC_APB1PeriphClockCmd(u32_t RCC_APB1Periph, false_true_et NewState){}
void RCC_APB2PeriphClockCmd(u32_t RCC_APB1Periph, false_true_et NewState){}
void GPIO_Init(u8_t* GPIOx, u8_t* GPIO_InitStruct){}







