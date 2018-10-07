/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/HAL/HAL_UART/HAL_UART.c $
*
*   \brief      UART interface module
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
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "C_defs.h"
#include "STDC.h"
#include "COMPILER_defs.h"
#include "HAL_I2C.h"
#include "HAL_TIM.h"
#include "NVM.h"
#include "main.h"
#include "HAL_UART.h"
#include "RFM69.h"


extern NVM_info_st NVM_info_s;

/* Module Identification for STDC_assert functionality */

STATIC const u8_t SERIAL_help_promt_s[] =
		"\r\nValid Commands\r\n"
		"-----------------------\r\n"
		"help:  help menu displayed\r\n"
		"reset: resets the core\r\n"
		"stats: displays stats\r\n"
		"batt:  returns the supply voltage\r\n"
		"temp:  returns the internal temperature\r\n"
		"debug: prints all debug information \r\n"
		"led:   < 0 or 1 > 0 - LED OFF, 1 - LED ON\r\n"
		"ver:   returns the SW version\r\n"
		"clocks: returns the current clock speed\r\n"
		"rf:    rf control < conf or tx >\r\n"
		"rand:  returns a random number generated by srand\r\n"
		"db:    < 0..3 > < 0 or 1 > ( eg db 1 1 turns DB 1 ON )\r\n"
		"sleep: <1..15000> sets the sleep time in secs\r\n"
		"stream: <0 1> Turns streaming mode on and off\r\n"
		"nvm:	prints the current NVM data\r\n";

STATIC const u8_t SERIAL_welcome_message_s[] =
		"\r\nHello.\r\nDebug Console is ready, Type 'help' for a list of commands\r\n"
		"All commands are case 'insensitive'\r\n"
		"/*********************************************************\r\n\r\n";

STATIC const u8_t SERIAL_buffer_reset_message_s[] =
		"\r\nERROR!... The cmd line has been reset!!\r\n\r\n";


STATIC const u8_t SERIAL_invalid_cmd_message_s[] =
		"\r\nERROR! that is not a valid command!\r\nPlease try again or type 'help' for help menu..\r\n\r\n";



STATIC false_true_et SERIAL_cr_received_s;
STATIC char SERIAL_rx_buf_idx_s;
STATIC char SERIAL_rx_buf_char_s;
STATIC char SERIAL_rx_buf_s[RX_BUF_SIZE] = {'\0'};
STATIC char SERIAL_tx_buf_s[TX_BUF_SIZE] = {'\0'};
STATIC false_true_et SERIAL_stream_mode_s;
STATIC false_true_et SERIAL_stream_triggered_s;


/***************************************************************************************************
**                              Data declarations and definitions                                 **
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
*   \note          Fixed baudrate for now at 9600 8N1
*
***************************************************************************************************/
void SERIAL_init( void )
{
	/* Enable GPIOA clock, should be enabled anyway but just in case */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

	/* Enable USART2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );

	/* NVIC Configuration */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure the GPIOs */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure USART1 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the USART2 */
	USART_InitTypeDef USART_InitStructure;

	/* USART1 configuration ------------------------------------------------------*/
	/* USART1 configured as follow:
		- BaudRate = 115200 baud
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
		- USART Clock disabled
		- USART CPOL: Clock is active low
		- USART CPHA: Data is captured on the middle
		- USART LastBit: The clock pulse of the last data bit is not output to
			the SCLK pin
	 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);

	/* Enable the USART2 Receive interrupt: this interrupt is generated when the
		USART2 receive data register is not empty */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	SERIAL_stream_mode_s = TRUE;
	SERIAL_cr_received_s = FALSE;
	SERIAL_rx_buf_idx_s = 0u;
	SERIAL_rx_buf_char_s = 0u;
	STDC_memset( SERIAL_rx_buf_s, '\0', sizeof( SERIAL_rx_buf_s ) );
	STDC_memset( SERIAL_tx_buf_s, '\0', sizeof( SERIAL_rx_buf_s ) );
	SERIAL_stream_triggered_s = FALSE;

	/* Finally send the welcome message */
	SERIAL_Send_data( SERIAL_welcome_message_s );
}



void SERIAL_msg_handler( void )
{
	if (SERIAL_cr_received_s == TRUE)
	{
		// Reset CR Flag
		SERIAL_cr_received_s = FALSE;

		if( strncmp(strlwr(SERIAL_rx_buf_s), "help\r", 5) == 0)
		{
			SERIAL_Send_data( SERIAL_help_promt_s );
		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "led" ) != 0)
		{
			char *result = strstr(SERIAL_rx_buf_s, "led") + 4;
			if( ( *result == '0' ) || ( strstr(result, "off") != 0 ) )
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
				sprintf( SERIAL_tx_buf_s, "\r\nLED turned off\r\n\r\n" );
				SERIAL_Send_data(SERIAL_tx_buf_s);
			}
			else if( ( * result == '1' ) || ( strstr(result, "on") != 0 ) )
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				sprintf( SERIAL_tx_buf_s, "\r\nLED turned on\r\n\r\n" );
				SERIAL_Send_data(SERIAL_tx_buf_s);
			}
			else
			{
				SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
			}
		}
		else if( strncmp(strlwr(SERIAL_rx_buf_s), "batt\r", 5) == 0)
		{
			u16_t battery_voltage = 3000u;

			sprintf( SERIAL_tx_buf_s, "\r\nSupply voltage is %dmV\r\n\r\n", battery_voltage );
			SERIAL_Send_data( SERIAL_tx_buf_s );
		}
		else if( strncmp(strlwr(SERIAL_rx_buf_s), "reset\r", 6) == 0)
		{
			SERIAL_Send_data("\r\nResetting Core....\r\n\r\n");
			NVIC_SystemReset();
		}
		else if( strncmp(strlwr(SERIAL_rx_buf_s), "ver\r", 4) == 0)
		{
			char version_num[5];

			//get_SW_version_number( version_num );

			sprintf( SERIAL_tx_buf_s, "\r\nSW version is %d.%d.%d\r\n\r\n", version_num[0], version_num[1], version_num[2] );
			SERIAL_Send_data( SERIAL_tx_buf_s );
		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "stream" ) != 0)
		{
			char *result = strstr(SERIAL_rx_buf_s, "stream") + 7;
			if( ( *result == '0' ) || ( strstr((char*)result, "off") != 0 ) )
			{
				SERIAL_stream_mode_s = FALSE;
				SERIAL_Send_data("\r\nStream mode turned off\r\n\r\n");
			}
			else if( ( * result == '1' ) || ( strstr(result, "on") != 0 ) )
			{
				SERIAL_stream_mode_s = TRUE;
				SERIAL_Send_data("\r\nStream mode turned on\r\n\r\n");
			}
			else
			{
				SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
			}
		}
		else if( strncmp(strlwr(SERIAL_rx_buf_s), "clocks\r", 7) == 0)
		{
			RCC_ClocksTypeDef RCC_Clocks;
			RCC_GetClocksFreq (&RCC_Clocks);

			sprintf( SERIAL_tx_buf_s, "SYSCLK:\t%dHz\r\nHCLK:\t%dHz\r\nPCLK1:\t%dHz\r\nPCLK2:\t%dHz\r\nADCCLK:\t%dHz\r\n",
					(int)RCC_Clocks.SYSCLK_Frequency, (int)RCC_Clocks.HCLK_Frequency, (int)RCC_Clocks.PCLK1_Frequency,
					(int)RCC_Clocks.PCLK2_Frequency, (int)RCC_Clocks.ADCCLK_Frequency );

			SERIAL_Send_data( SERIAL_tx_buf_s );
		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "db" ) != 0)
		{
			char *result = strstr(SERIAL_rx_buf_s, "db") + 3;
			char *result1 = strstr(SERIAL_rx_buf_s, "db") + 5;

			switch( *result )
			{
				case '0':
					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
					{
					}
					else
					{
					}
					break;

				case '1':
					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
					{
					}
					else
					{
					}
					break;

				case '2':
					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
					{
					}
					else
					{
					}
					break;

				case '3':
					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
					{
					}
					else
					{
					}
					break;

				default:
					SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
					break;
			}
		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "sleep" ) != 0)
		{
			u32_t val;

			char *result = strstr(SERIAL_rx_buf_s, "sleep") + 6;

			val = atoi(result);

			if( val > 15300 )
			{
				val = 15300;
				sprintf( SERIAL_tx_buf_s, "Selection too high, Max = 15300, using Max instead :)\r\n\r\n" );
				SERIAL_Send_data( SERIAL_tx_buf_s );
			}
			else
			{
				sprintf( SERIAL_tx_buf_s, "Sleep time has been set to %d secs\r\n\r\n", val );
				SERIAL_Send_data( SERIAL_tx_buf_s );

				RTC_set_wakeup_time( val );

				/* change the value and write to NVM */
				NVM_info_s.NVM_generic_data_blk_s.sleep_time = val;
				NVM_request_flush();
			}

		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "rf" ) != 0)
		{
			u8_t val;
			
			char *sub_string = strstr(SERIAL_rx_buf_s, "rf") + 3;
			
			if( ( strstr(sub_string, "conf") != 0 ) )
			{
				sub_string = strstr(SERIAL_rx_buf_s, "rf") + 8;
				
				val = atoi(sub_string);
				
				if( val > MAX_NUM_RF_CONFIGS )
				{
					/* Default them to config */
					val = 0u;
					
					sprintf( SERIAL_tx_buf_s, "Selection not valid.. config can be between 0..%d\r\n", MAX_NUM_RF_CONFIGS );
					SERIAL_Send_data( SERIAL_tx_buf_s );
				}
				else
				{
					sprintf( SERIAL_tx_buf_s, "Rf config %d selected\r\n", val );
					SERIAL_Send_data( SERIAL_tx_buf_s );
				}
			}
			else if( ( strstr(sub_string, "tx") != 0 ) )
			{
				*sub_string = strstr(SERIAL_rx_buf_s, "tx") + 3;
			}
			else if( ( strstr(sub_string, "read") != 0 ) )
			{
				u8_t read_data[79];
				u8_t i;

				RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGOPMODE, read_data, sizeof( read_data ) );

				for( i = 0u; i < 79; i++ )
				{
					sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", i+1, read_data[i] );
					SERIAL_Send_data( SERIAL_tx_buf_s );
				}

				sprintf( SERIAL_tx_buf_s, "\r\n" );
				SERIAL_Send_data( SERIAL_tx_buf_s );
			}
			else if( ( strstr(sub_string, "fifo") != 0 ) )
			{
				u8_t rfm69_fifo_regs[ RFM69_MAX_DATA_LEN ];
				u8_t i;

				RFM69_read_FIFO_register( rfm69_fifo_regs );

				for( i = 0u; i < RFM69_MAX_DATA_LEN; i++ )
				{
					sprintf( SERIAL_tx_buf_s, "\r\nFifo Reg %02d is %02X", i, rfm69_fifo_regs[i] );
					SERIAL_Send_data( SERIAL_tx_buf_s );
				}

				sprintf( SERIAL_tx_buf_s, "\r\n\r\n" );
				SERIAL_Send_data( SERIAL_tx_buf_s );
			}
			else if( ( strstr(sub_string, "power") != 0 ) )
			{
				sub_string = strstr(SERIAL_rx_buf_s, "rf") + 9;

				val = atoi(sub_string);

				if( val > RFM69_MAX_TX_POWER_LEVEL )
				{
					sprintf( SERIAL_tx_buf_s, "\r\nRF TX power selection %d is not valid\r\nAuto adjusting the RF TX power selection...\r\n", val );
					SERIAL_Send_data( SERIAL_tx_buf_s );

					/* Default them to config */
					val = RFM69_set_PA_level;
				}

				sprintf( SERIAL_tx_buf_s, "\r\nRF TX power %d selected\r\n", val );
				SERIAL_Send_data( SERIAL_tx_buf_s );

				RFM69_set_PA_level( val );

				NVM_info_s.NVM_generic_data_blk_s.tx_power_level = val;
				NVM_request_flush();

				sprintf( SERIAL_tx_buf_s, "\r\nActual RF TX power read back : 0x%02X \r\n", val );
				SERIAL_Send_data( SERIAL_tx_buf_s );
			}
			else if (( strstr(sub_string, "id") != 0 ) )
			{
				sub_string = strstr(SERIAL_rx_buf_s, "rf") + 7;

				val = atoi(sub_string);
				NVM_info_s.NVM_generic_data_blk_s.node_id = val;
				NVM_request_flush();

				RFM69_set_own_node_address( val );
			}
			else if( ( strstr(sub_string, "test") != 0 ) )
			{
				u8_t i;

				if( RFM69_read_version_num() != RFM69_VERSION )
				{
				   STDC_basic_assert();

				   sprintf( SERIAL_tx_buf_s, "\r\nReading back the RFM69 version number failed....fuck!! :( \r\n" );
				   SERIAL_Send_data( SERIAL_tx_buf_s );
				}
				else
				{
					sprintf( SERIAL_tx_buf_s, "\r\nVersion number check passed\r\nWriting down the RFM69 Register config....\r\n" );
					SERIAL_Send_data( SERIAL_tx_buf_s );
					STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

					/* Fire down a config of registers */
					RFM69_set_configuration( RFM69_433Mhz_OOK );

					u32_t freq;
					freq = RFM69_read_rf_carrier_freq();
					sprintf( SERIAL_tx_buf_s, "\r\nRF carrier frequency is %dHz\r\n", freq );
					SERIAL_Send_data( SERIAL_tx_buf_s );
					STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

					sprintf( SERIAL_tx_buf_s, "\r\nReading back the RFM69 Register config....\r\n" );
					SERIAL_Send_data( SERIAL_tx_buf_s );
					STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

					u8_t read_data[79];
					RFM69_register_data_st write_data[79];

					RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGOPMODE, read_data, sizeof( read_data ) );
					RFM69_get_configuration( RFM69_433Mhz_OOK, &write_data );

					for( i = 0u; i < 79; i++ )
					{
						sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is write:%02X read:%02X", write_data[i].RFM69_register, write_data[i].register_data, read_data[i] );
						SERIAL_Send_data( SERIAL_tx_buf_s );
					}

					sprintf( SERIAL_tx_buf_s, "\r\n\r\n" );
					SERIAL_Send_data( SERIAL_tx_buf_s );
				}
			}
			else if(( strstr(sub_string, "stats") != 0 ) )
			{
				u32_t cnt;
				u32_t time;
				f32_t percent;

				cnt = RFM69_get_received_packet_cnt();
				time = HAL_TIM_get_time();

				percent = ( ( (f32_t)cnt / (f32_t)time ) * 100.0f );

				sprintf( SERIAL_tx_buf_s, "\r\nCurrent received packet count:\t\t\t%d", cnt );
				SERIAL_Send_data( SERIAL_tx_buf_s );
				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

				sprintf( SERIAL_tx_buf_s, "\r\nSeconds elapsed since first frame received:\t%d", time );
				SERIAL_Send_data( SERIAL_tx_buf_s );
				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

				sprintf( SERIAL_tx_buf_s, "\r\nNo of missed frames:\t\t\t\t%d", ( time - cnt ) );
				SERIAL_Send_data( SERIAL_tx_buf_s );
				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

				sprintf( SERIAL_tx_buf_s, "\r\nPacket reception status:\t\t\t%d%%\r\n", (u8_t)percent );
				SERIAL_Send_data( SERIAL_tx_buf_s );
				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
			}
			else
			{
				sprintf( SERIAL_tx_buf_s, "Selection not valid please select \"conf\" or \"tx\"\r\n");
				SERIAL_Send_data( SERIAL_tx_buf_s );
				sprintf( SERIAL_tx_buf_s, "eg conf 0 = loads rf config 0, or others if applicable\r\n");
				SERIAL_Send_data( SERIAL_tx_buf_s );
				sprintf( SERIAL_tx_buf_s, "eg rf tx = triggers a packet of rf to be sent\r\n");
				SERIAL_Send_data( SERIAL_tx_buf_s );
			}
		}
		else if( strncmp(strlwr(SERIAL_rx_buf_s), "rand\r", 5) == 0)
		{
			u32_t random_number;

			/* Set the seed */
			srand( get_counter() );

			/* Grab the now "random :)" number */
			random_number = rand()%100;

			while( random_number == 0 )
			{
				random_number = rand()%100;
			}

			sprintf( SERIAL_tx_buf_s, "Random number:\t%d\r\n", random_number );
			SERIAL_Send_data( SERIAL_tx_buf_s );
		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "time" ) != 0)
		{
			u8_t hours;
			u8_t mins;
			u8_t secs;

			char *sub_string = strstr(SERIAL_rx_buf_s, "time") + 5;
			hours = (u8_t)atoi(sub_string);

			*sub_string = strstr(SERIAL_rx_buf_s, "time") + 8;
			mins = (u8_t)atoi(sub_string);

			*sub_string = strstr(SERIAL_rx_buf_s, "time") + 11;
			secs = (u8_t)atoi(sub_string);

			sprintf( SERIAL_tx_buf_s, "\r\nTime has been set to\r\nHours:\t%d\r\nMins:\t%d\r\n\r\nSecs:\t%d", hours, mins, secs );
		}
		else if( strstr(strlwr(SERIAL_rx_buf_s), "nvm" ) != 0)
		{
			sprintf( SERIAL_tx_buf_s, "chksum:\t0x%02X\r\nVers:\t%d\r\nwrites:\t%d\r\nSleep time:\t%d\r\n", NVM_info_s.checksum, NVM_info_s.version,
																										  NVM_info_s.write_count,
																										  NVM_info_s.NVM_generic_data_blk_s.sleep_time );
			SERIAL_Send_data( SERIAL_tx_buf_s );
		}
		else if ( strstr(strlwr(SERIAL_rx_buf_s), "test" ) != 0)
		{

		}
		else
		{
			SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
		}

		SERIAL_clear_RXBuffer();
		SERIAL_clear_TXBuffer();
	}
	else if( SERIAL_stream_mode_s == TRUE )
	{
		if( SERIAL_stream_triggered_s == TRUE )
		{
			SERIAL_stream_triggered_s = FALSE;

			u32_t cnt;
			u32_t time;
			f32_t percent;

			cnt = RFM69_get_received_packet_cnt();
			time = HAL_TIM_get_time();

			percent = ( ( (f32_t)cnt / (f32_t)time ) * 100.0f );

			sprintf( SERIAL_tx_buf_s, "\r\nCurrent received packet count:\t\t\t%d", cnt );
			SERIAL_Send_data( SERIAL_tx_buf_s );
			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

			sprintf( SERIAL_tx_buf_s, "\r\nSeconds elapsed since first frame received:\t%d", time );
			SERIAL_Send_data( SERIAL_tx_buf_s );
			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

			sprintf( SERIAL_tx_buf_s, "\r\nNo of missed frames:\t\t\t\t%d", ( time - cnt ) );
			SERIAL_Send_data( SERIAL_tx_buf_s );
			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );

			sprintf( SERIAL_tx_buf_s, "\r\nPacket reception status:\t\t\t%d%%\r\n", (u8_t)percent );
			SERIAL_Send_data( SERIAL_tx_buf_s );
			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
		}
	}
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
void SERIAL_Send_data(const char *pucBuffer)
{
    while (*pucBuffer)
    {
        USART_SendData(USART2, *pucBuffer);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        }
        pucBuffer += 1;
    }
}



void SERIAL_clear_RXBuffer(void)
{
	for ( SERIAL_rx_buf_idx_s = 0; SERIAL_rx_buf_idx_s < RX_BUF_SIZE; SERIAL_rx_buf_idx_s++ )
	{
		SERIAL_rx_buf_s[SERIAL_rx_buf_idx_s] = '\0';
	}
	SERIAL_rx_buf_idx_s = 0;
}



void SERIAL_clear_TXBuffer(void)
{
	char i = 0;
	for ( i = 0; i < TX_BUF_SIZE; i++ )
	{
		SERIAL_tx_buf_s[i] = '\0';
	}
}




void SERIAL_trigger_stream_output( void )
{
	SERIAL_stream_triggered_s = TRUE;
}


//
///***************************************************************************************************
//**                              ISR Handlers                                                      **
//***************************************************************************************************/
void USART2_IRQHandler(void)
{
    if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
		SERIAL_rx_buf_char_s = USART_ReceiveData(USART2);
		SERIAL_rx_buf_s[SERIAL_rx_buf_idx_s] = SERIAL_rx_buf_char_s;
		SERIAL_rx_buf_idx_s++;

		if (SERIAL_rx_buf_char_s != '\r')
		{
			if (SERIAL_rx_buf_idx_s >= RX_BUF_SIZE)
			{
				SERIAL_Send_data( SERIAL_buffer_reset_message_s );
				SERIAL_clear_RXBuffer();
			}
		}
		else
		{
			SERIAL_cr_received_s = TRUE;
		}

		//Echo
		SERIAL_Send_data( &SERIAL_rx_buf_char_s );
	}
}


///****************************** END OF FILE *******************************************************/
