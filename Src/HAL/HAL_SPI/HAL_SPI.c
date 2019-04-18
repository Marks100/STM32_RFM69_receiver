/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/HAL/HAL_SPI/HAL_SPI.c $
*
*   \brief      SPI interface module
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
*         (C) $Date:: 2014#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"

#include "C_defs.h"
#include "COMPILER_defs.h"
#include "HAL_BRD.h"
#include "HAL_SPI.h"



/* Module Identification for STDC_assert functionality */
#define STDC_MODULE_ID   STDC_MOD_HAL_SPI

static false_true_et HAL_SPI1_initialised = FALSE;
static false_true_et HAL_SPI2_initialised = FALSE;






/***************************************************************************************************
**                              Data declaratons and definitions                                 **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/
/*!
****************************************************************************************************
*
*   \brief         Module (re-)initialisation function
*
*   \author        MS
*
*   \return        none
*
*   \note          Fixed baudrate for now at 5MHz 8N1
*
***************************************************************************************************/
void HAL_SPI1_init( void )
{
	/* Enable SPI clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );

	/* Enable the GPIOA clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

	/* Configure the GPIOs */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Setup the SPI pins (PA5, PA6, PA7 ) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitTypeDef   SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0xAAAA;

	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1 */
	SPI_Cmd(SPI1, ENABLE);

    HAL_SPI1_initialised = TRUE;
}


u16_t SPITransmittedValue[2] = {0xFF00,0x00FF};

/*!
****************************************************************************************************
*
*   \brief         Module (re-)initialisation function
*
*   \author        MS
*
*   \return        none
*
*   \note          Fixed baudrate for now at 5MHz 8N1
*
***************************************************************************************************/
void HAL_SPI2_init( void )
{
	/* Enable SPI clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );

	/* Enable the GPIOB clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

	// Reset SPI Interface
	SPI_I2S_DeInit(SPI2);

	/* Configure the GPIOs */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Setup the SPI pins ( PB13, PB14, PB15 ) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_InitTypeDef   SPI_InitStructure;

	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0xAAAA;

	SPI_Init(SPI2, &SPI_InitStructure);

	//Enable DMA1 clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef     DMA_InitStructure;

	//Configure DMA1 - Channel5== (memory -> SPI)
	DMA_DeInit(DMA1_Channel5); //Set DMA registers to default values
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR; //Address of peripheral the DMA must map to
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&SPITransmittedValue[0]; //Variable from which data will be transmitted
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 2; //Buffer size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure); //Initialise the DMA
	DMA_Cmd(DMA1_Channel5, ENABLE); //Enable the DMA1 - Channel5

	/* Enable SPI2 */
	SPI_Cmd(SPI2, ENABLE);

	// Enable the SPI2 TX DMA requests
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

    HAL_SPI2_initialised = TRUE;
}




void HAL_SPI1_de_init( void )
{
	/* Disable SPI1 */
	SPI_Cmd(SPI1, DISABLE);

	SPI_I2S_DeInit(SPI1);

	/* Disable SPI clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, DISABLE );

    HAL_SPI1_initialised = FALSE;
}



void HAL_SPI2_de_init( void )
{
	/* Disable SPI1 */
	SPI_Cmd(SPI2, DISABLE);

	SPI_I2S_DeInit(SPI2);

	/* Disable SPI clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, DISABLE );

    HAL_SPI2_initialised = FALSE;
}



/*!
****************************************************************************************************
*
*   \brief         Return the init status of the SPI module
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
false_true_et HAL_SPI1_get_init_status( void )
{
    return ( HAL_SPI1_initialised );
}

/*!
****************************************************************************************************
*
*   \brief         Return the init status of the SPI module
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
false_true_et HAL_SPI2_get_init_status( void )
{
    return ( HAL_SPI2_initialised );
}


/*!
****************************************************************************************************
*
*   \brief         Writes a buffer of information out to UART
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
u8_t HAL_SPI1_write_and_read_data( u8_t tx_data )
{
    u8_t return_value;

	/* First lets do a dummy read to make sure that the interrupt
	flag is clear and that the buffer is empty*/

	/* Send SPI1 data */
	SPI_I2S_SendData(SPI1, tx_data);

	/* Wait for the SPI busy flag to clear */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

	/* Read the buffer on the SPI receive register */
	return_value = SPI_I2S_ReceiveData( SPI1 );

    return ( return_value );
}



/*!
****************************************************************************************************
*
*   \brief         Writes a buffer of information out to UART
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
u8_t HAL_SPI2_write_and_read_data( u8_t tx_data )
{
    u8_t return_value;

	/* First lets do a dummy read to make sure that the interrupt
	flag is clear and that the buffer is empty*/

	/* Send SPI2 data */
	SPI_I2S_SendData(SPI2, tx_data);

	/* Wait for the SPI busy flag to clear */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);

	/* Read the buffer on the SPI receive register */
	return_value = SPI_I2S_ReceiveData( SPI2 );

    return ( return_value );
}



/*!
****************************************************************************************************
*
*   \brief
*
*   \author        FK
*
*   \return        none
*
***************************************************************************************************/
void HAL_SPI1_disable_rx_interrupt( void )
{

}

/*!
****************************************************************************************************
*
*   \brief
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void HAL_SPI1_enable_rx_interrupt( void )
{

}


/*!
****************************************************************************************************
*
*   \brief
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void HAL_SPI1_enable_tx_interrupt( void )
{
}


/*!
****************************************************************************************************
*
*   \brief
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void HAL_SPI1_disable_tx_interrupt( void )
{
}



/*!
****************************************************************************************************
*
*   \brief
*
*   \author        MS
*
*   \return        none
*
***************************************************************************************************/
void HAL_SPI1_clear_receive_spi_buffer( void )
{

}



/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              ISR Handlers                                                      **
***************************************************************************************************/

/****************************** END OF FILE *******************************************************/
