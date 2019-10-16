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
#include "C_defs.h"
#include "STDC.h"
#include "NVM.h"
#include "HAL_I2C.h"
#include "BMP280.h"


extern NVM_info_st NVM_info_s;

BMP280_calib_st BMP280_calib_s;
float BMP280_temp_c;


/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */


void BMP280_init( void )
{
	STDC_memset( &BMP280_calib_s, 0x00, sizeof( BMP280_calib_st ));
	BMP280_temp_c = S16_T_MAX;
}


void BMP280_setup_default_registers( void )
{
	u8_t register_data = 0u;
	
	//BMP280_set_mode( BMP280_NORMAL_MODE );

	register_data = BMP280_CONFIG_BIT_MASK;
	BMP280_set_config( &register_data );

	register_data = BMP280_MEAS_BIT_MASK;
	BMP280_set_ctrl_meas( &register_data );
}


u8_t BMP280_read_id( void )
{
	u8_t id;

	HAL_I2C_read_register( BMP280_I2C_ADDR, BMP280_ID, &id );

	return ( id );
}



void BMP280_reset( void )
{
	u8_t register_data = BMP280_RESET_CMD;
	HAL_I2C_write_single_register( BMP280_I2C_ADDR, BMP280_RESET, &register_data );
}


u8_t BMP280_read_status( void )
{
	u8_t status;

	HAL_I2C_read_register( BMP280_I2C_ADDR, BMP280_STATUS, &status );

	return( status );
}


void BMP280_read_calib_values( void )
{
	u8_t calib_data[24];

	HAL_I2C_read_multiple_registers( BMP280_I2C_ADDR, BMP280_CALIB_START, calib_data, sizeof( calib_data ) );

	BMP280_calib_s.calib_T1 = ( ( calib_data[1] << 8u ) | calib_data[0] );
	BMP280_calib_s.calib_T2 = ( ( calib_data[3] << 8u ) | calib_data[2] );
	BMP280_calib_s.calib_T3 = ( ( calib_data[5] << 8u ) | calib_data[4] );

	BMP280_calib_s.calib_P1 = ( ( calib_data[7] << 8u ) | calib_data[6] );
	BMP280_calib_s.calib_P2 = ( ( calib_data[9] << 8u ) | calib_data[8] );
	BMP280_calib_s.calib_P3 = ( ( calib_data[11] << 8u ) | calib_data[10] );
	BMP280_calib_s.calib_P4 = ( ( calib_data[13] << 8u ) | calib_data[12] );
	BMP280_calib_s.calib_P5 = ( ( calib_data[15] << 8u ) | calib_data[14] );
	BMP280_calib_s.calib_P6 = ( ( calib_data[17] << 8u ) | calib_data[16] );
	BMP280_calib_s.calib_P7 = ( ( calib_data[19] << 8u ) | calib_data[18] );
	BMP280_calib_s.calib_P8 = ( ( calib_data[21] << 8u ) | calib_data[20] );
	BMP280_calib_s.calib_P9 = ( ( calib_data[23] << 8u ) | calib_data[22] );
}


u32_t BMP280_read_raw_temp_counts( void )
{
	u8_t temp_data[3];
	u32_t temp_value;

	HAL_I2C_read_multiple_registers( BMP280_I2C_ADDR, BMP280_TEMP_MSB, temp_data, sizeof( temp_data ) );

	temp_value = ( ( temp_data[0] << 16u ) | ( temp_data[1] << 8 ) | temp_data[2] );

	return ( temp_value >> 4u );
}


u32_t BMP280_read_raw_pressure_counts( void )
{
	u8_t pressure_data[3];
	u32_t pressure_value;

	HAL_I2C_read_multiple_registers( BMP280_I2C_ADDR, BMP280_PRESS_MSB, pressure_data, sizeof( pressure_data ) );

	pressure_value = ( ( pressure_data[0] << 16u ) | ( pressure_data[1] << 8 ) | pressure_data[2] );

	return ( pressure_value >> 4u );
}



void BMP280_convert( s32_t* temperature, s32_t* pressure)
{
  unsigned long adc_T;
  unsigned long adc_P;

  adc_P = BMP280_read_raw_pressure_counts();
  adc_T = BMP280_read_raw_temp_counts();

  double var1, var2, p, t_fine;
  var1 = (((double)adc_T)/16384.0 - ((double)BMP280_calib_s.calib_T1)/1024.0) * ((double)BMP280_calib_s.calib_T2);
  var2 = ((((double)adc_T)/131072.0 - ((double)BMP280_calib_s.calib_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) BMP280_calib_s.calib_T1)/8192.0)) * ((double)BMP280_calib_s.calib_T3);
  t_fine = (var1 + var2);
  *temperature = (int32_t) (t_fine*10 / 5120.0);

  var1 = ((double)t_fine/2.0) - 64000.0;
  var2 = var1 * var1 * ((double)BMP280_calib_s.calib_P6) / 32768.0;
  var2 = var2 + var1 * ((double)BMP280_calib_s.calib_P5) * 2.0;
  var2 = (var2/4.0)+(((double)BMP280_calib_s.calib_P4) * 65536.0);
  var1 = (((double)BMP280_calib_s.calib_P3) * var1 * var1 / 524288.0 + ((double)BMP280_calib_s.calib_P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0)*((double)BMP280_calib_s.calib_P1);
  if (var1 == 0.0)
  {
    return; // avoid exception caused by division by zero
  }
  p = 1048576.0 - (double)adc_P;
  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = ((double)BMP280_calib_s.calib_P9) * p * p / 2147483648.0;
  var2 = p * ((double)BMP280_calib_s.calib_P8) / 32768.0;
  p = (p + (var1 + var2 + ((double)BMP280_calib_s.calib_P7)) / 16.0);

  *pressure = (long)p;
}



void BMP280_set_mode( BMP280_operating_modes_et mode )
{
  u8_t register_data = 0u;

  HAL_I2C_read_register( BMP280_I2C_ADDR, BMP280_CTRL_MEAS, &register_data );

  register_data &= ~( 0x03 );
  register_data |= mode;

  HAL_I2C_write_single_register( BMP280_I2C_ADDR, BMP280_CTRL_MEAS, &register_data );
}


pass_fail_et BMP280_self_check( void )
{
	/* Assume fail until pass */
	pass_fail_et status = FAIL;

	if( BMP280_DEVICE_ID == BMP280_read_id() )
	{
		status = PASS;
	}
	return( status );
}



void BMP280_shutdown( void )
{
	BMP280_set_mode( BMP280_SLEEP_MODE );
}


void BMP280_set_ctrl_meas( u8_t* data )
{
	HAL_I2C_write_single_register( BMP280_I2C_ADDR, BMP280_CTRL_MEAS, data );	
}


void BMP280_set_config( u8_t* data )
{
	HAL_I2C_write_single_register( BMP280_I2C_ADDR, BMP280_CONFIG, data );	
}



float BMP280_take_temp_measurement( void )
{
	s32_t BMP280_pressure_reading_s;
	s32_t BMP280_temperature_reading_s;

	BMP280_convert( &BMP280_temperature_reading_s, &BMP280_pressure_reading_s );

	BMP280_temp_c = ( BMP280_temperature_reading_s / 10.0 );
	
	return( BMP280_temp_c );
}


void BMP280_read_all_regs( void )
{
	u8_t regs[8];

	HAL_I2C_read_multiple_registers( BMP280_I2C_ADDR, BMP280_STATUS, regs, 8 );
}


///****************************** END OF FILE *******************************************************/
