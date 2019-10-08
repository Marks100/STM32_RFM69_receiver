/*! \file
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*   \brief      BMP280
*/

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#include "C_defs.h"
#include "STDC.h"
#include "HAL_BRD.h"
#include "NVM.h"
#include "COMPILER_defs.h"
#include "HAL_I2C.h"
#include "BMP280.h"
#include "TEMP.h"



extern NVM_info_st NVM_info_s;

s16_t TEMP_temperature_c_s;
TEMP_state_et TEMP_state_s;
u16_t TEMP_timeout_s;
u16_t TEMP_fail_cnt_s;

/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */


void TEMP_init( void )
{
	TEMP_temperature_c_s = S16_T_MAX;
	TEMP_state_s = TEMP_INIT;
	TEMP_timeout_s = TEMP_TIMEOUT;
	TEMP_fail_cnt_s = 0u;
}



void TEMP_cyclic( void )
{
	switch( TEMP_get_state() )
	{
		case TEMP_INIT:
			BMP280_init();
			
			BMP280_reset();
			
			TEMP_set_state( TEMP_SETUP );
		break;
		
		case TEMP_SETUP:
			BMP280_setup_default_registers();
			
			TEMP_set_state( TEMP_RUNNING );
		break;
		
		case TEMP_RUNNING:
			TEMP_take_temp_measurement( &TEMP_temperature_c_s );
		break;
		
		case TEMP_ERROR:
			BMP280_reset();
			
			TEMP_increment_fail_cnt();
			
			TEMP_set_state( TEMP_INIT );
		break;

		default:
			TEMP_set_state( TEMP_SETUP );
		break;
	}
	TEMP_self_check();
}


TEMP_state_et TEMP_get_state( void )
{
	return( TEMP_state_s );
}

void TEMP_set_state( TEMP_state_et state )
{
	TEMP_state_s = state;
}


pass_fail_et TEMP_self_check( void )
{
	pass_fail_et test;
	
	test = BMP280_self_check();
	
	if( test == FAIL )
	{
		TEMP_set_state( TEMP_ERROR );
	}

	return( test );
}


void TEMP_increment_fail_cnt( void )
{
	if( TEMP_fail_cnt_s <= U16_T_MAX )
	{
		TEMP_fail_cnt_s++; 
	}
}

void TEMP_take_temp_measurement( s16_t* TEMP_temperature_c_s )
{
	s16_t temp;

	temp = BMP280_take_temp_measurement();
	TEMP_temperature_c_s = &temp;
}

pass_fail_et TEMP_get_last_temp_measurement( s16_t* temp )
{
	pass_fail_et status = FAIL;
	
	if( TEMP_temperature_c_s != S16_T_MAX )
	{
		temp = &TEMP_temperature_c_s;
		status = PASS;
	}
	return( status );
} 



///****************************** END OF FILE *******************************************************/














//
//
//
//#include "stm32f10x_i2c.h"
//#include "bmp280.h"
//#include <math.h>
//
//unsigned short dig_T1;
//short dig_T2;
//short dig_T3;
//unsigned short dig_P1;
//
//short dig_P2;
//short dig_P3;
//short dig_P4;
//short dig_P5;
//short dig_P6;
//short dig_P7;
//short dig_P8;
//short dig_P9;
//
//short bmp280ReadShort(unsigned char address)
//{
//  short msb=0;
//  short lsb=0;
//
//  I2C_AcknowledgeConfig(I2C1,ENABLE);
//  I2C_GenerateSTART(I2C1,ENABLE);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//  I2C_Send7bitAddress(I2C1, BMP280_addr, I2C_Direction_Transmitter);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//  I2C_SendData(I2C1,address);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//  I2C_GenerateSTART(I2C1,ENABLE);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//  I2C_Send7bitAddress(I2C1, BMP280_addr, I2C_Direction_Receiver);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  lsb = I2C_ReceiveData(I2C1);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  msb = I2C_ReceiveData(I2C1);
//
//  I2C_GenerateSTOP(I2C1,ENABLE);
//  I2C_AcknowledgeConfig(I2C1,DISABLE);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  I2C_ReceiveData(I2C1);
//
//  return (msb << 8) | lsb;
//}
//
//unsigned long bmp280ReadLong(unsigned char address)
//{
//  unsigned long result=0;
//
//  unsigned long msb=0;
//  unsigned long lsb=0;
//  unsigned long xsb=0;
//
//  I2C_AcknowledgeConfig(I2C1,ENABLE);
//  I2C_GenerateSTART(I2C1,ENABLE);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//  I2C_Send7bitAddress(I2C1, BMP280_addr, I2C_Direction_Transmitter);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//  I2C_SendData(I2C1,address);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//  I2C_GenerateSTART(I2C1,ENABLE);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//  I2C_Send7bitAddress(I2C1, BMP280_addr, I2C_Direction_Receiver);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  msb = I2C_ReceiveData(I2C1);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  lsb = I2C_ReceiveData(I2C1);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  xsb = I2C_ReceiveData(I2C1);
//
//  I2C_GenerateSTOP(I2C1,ENABLE);
//  I2C_AcknowledgeConfig(I2C1,DISABLE);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//  I2C_ReceiveData(I2C1);
//
//  result = (msb << 16) | (lsb << 8) | xsb;
//
//  return (result >> 4);
//}
//
//void bmp280WriteByte(unsigned char address, unsigned char data)
//{
//  I2C_GenerateSTART(I2C1,ENABLE);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//  I2C_Send7bitAddress(I2C1, BMP280_addr, I2C_Direction_Transmitter);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//  I2C_SendData(I2C1,address);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//  I2C_SendData(I2C1,data);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//  I2C_GenerateSTOP(I2C1,ENABLE);
//
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//}
////----------------------------------------
//
//void bmp280Convert(long* temperature, long* pressure)
//{
//  unsigned long adc_T;
//  unsigned long adc_P;
//  adc_T = bmp280ReadLong(BMP280_REG_RESULT_TEMPRERATURE);
//  adc_P = bmp280ReadLong(BMP280_REG_RESULT_PRESSURE);
//
//  double var1, var2, p, t_fine;
//  var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
//  var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
//  t_fine = (var1 + var2);
//  *temperature = (int32_t) (t_fine*10 / 5120.0);
//
//  var1 = ((double)t_fine/2.0) - 64000.0;
//  var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
//  var2 = var2 + var1 * ((double)dig_P5) * 2.0;
//  var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
//  var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
//  var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
//  if (var1 == 0.0)
//  {
//    return; // avoid exception caused by division by zero
//  }
//  p = 1048576.0 - (double)adc_P;
//  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
//  var1 = ((double)dig_P9) * p * p / 2147483648.0;
//  var2 = p * ((double)dig_P8) / 32768.0;
//  p = (p + (var1 + var2 + ((double)dig_P7)) / 16.0);
//
//  *pressure = (long)p;
//}
////----------------------------------------
//
//void BMP280_Init(void)
//{
//  dig_T1 = bmp280ReadShort(0x88);//dig_T1
//  dig_T2 = bmp280ReadShort(0x8A);//dig_T2
//  dig_T3 = bmp280ReadShort(0x8C);//dig_T3
//  dig_P1 = bmp280ReadShort(0x8E);//dig_P1
//  dig_P2 = bmp280ReadShort(0x90);//dig_P2
//  dig_P3 = bmp280ReadShort(0x92);//dig_P3
//  dig_P4 = bmp280ReadShort(0x94);//dig_P4
//  dig_P5 = bmp280ReadShort(0x96);//dig_P5
//  dig_P6 = bmp280ReadShort(0x98);//dig_P6
//  dig_P7 = bmp280ReadShort(0x9A);//dig_P7
//  dig_P8 = bmp280ReadShort(0x9C);//dig_P8
//  dig_P9 = bmp280ReadShort(0x9E);//dig_P9
//
//  bmp280WriteByte(BMP280_REG_CONFIG, BMP280_CONFIG);
//  bmp280WriteByte(BMP280_REG_CONTROL, BMP280_MEAS);
//}
