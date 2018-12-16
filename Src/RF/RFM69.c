/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL:
*
*   \brief      RF module
*/
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#ifndef UNIT_TEST
#endif

#ifdef GCC_TEST
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif // GCC_TEST

#include "PROJ_config.h"
#include "C_defs.h"
#include "STDC.h"
#include "COMPILER_defs.h"
#include "HAL_BRD.h"
#include "HAL_SPI.h"
#include "HAL_TIM.h"
#include "main.h"
#include "nvm.h"
#include "RFM69.h"
#include "RFM69_Registers.h"


#define STDC_MODULE_ID   STDC_MOD_RF_DECODE



extern NVM_info_st NVM_info_s;

STATIC RFM69_data_packet_st RFM69_data_packet_s;

STATIC u8_t send_data[RFM69_MAX_PAYLOAD_LEN] =
{
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
    28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
    52,53,54,55,56,57,58,59,60,61
};

false_true_et  RFM69_init_s;
false_true_et  RFM69_packet_sent_s;
false_true_et  RFM69_packet_received_s;
u8_t           RFM69_tx_power_level_s;
u32_t          RFM69_received_packet_cnt_s;
f32_t          RFM69_packet_reception_percent_s;
s8_t           RFM69_last_rssi_s;
u32_t          RFM69_packet_received_timestamp_s;
RFM69_state_et RFM69_state_s;


/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/

void RFM69_init( void )
{
    RFM69_state_s =
	RFM69_tx_power_level_s = NVM_info_s.NVM_generic_data_blk_s.tx_power_level;
	RFM69_received_packet_cnt_s = 0u;

	RFM69_packet_sent_s = FALSE;
	RFM69_packet_received_s = FALSE;

	RFM69_packet_reception_percent_s = 0.0f;
	RFM69_last_rssi_s = -127;
	RFM69_packet_received_timestamp_s = 0u;
}



void RFM69_tick( void )
{
    switch( RFM69_state_s )
    {
        case RFM69_MGR_INITIALISING:
            RFM69_state_s = RFM69_MGR_RX_MODE;
            break;
        break;

        case RFM69_MGR_SETUP_TX_MODE:
            RFM69_setup_receive_mode();
            RFM69_state_s = RFM69_MGR_RX_MODE;
        break;

        case RFM69_MGR_TX_MODE:
        break;
        case RFM69_MGR_SETUP_RX_MODE:
        break;

        case RFM69_MGR_RX_MODE:
            RFM69_receive_frame();
        break;

        case RFM69_MGR_SLEEP_MODE:
        break;

        case RFM69_MGR_IDLE:
        break;

        case RFM69_MGR_RF_RESET:
        break;
    }
}



/*!
****************************************************************************************************
*
*   \brief         Module (re-)initialisation function
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RFM69_setup_receive_mode( void )
{
	u8_t read_data[50];

	/* power up the RF chip */
	RFM69_set_enable_pin_state( HIGH );

	RFM69_set_reset_pin_state( HIGH );
    delay_us(2000);
    RFM69_set_reset_pin_state( LOW );

    /* Give the RF chip time to stabilise */
    delay_us(6000);

    RFM69_set_operating_mode( RFM69_SLEEP_MODE );

	/* Go through the elements and reset them to 0xFF */
	STDC_memset( &RFM69_data_packet_s, 0xFF, sizeof( RFM69_data_packet_st ) );

	/* Firstly check that the version number is correct */
	if( RFM69_read_version_num() != RFM69_VERSION )
	{
	   STDC_basic_assert();
	}

	if( RFM69_read_reserved_registers() == PASS )
	{
		/* Fire down a config of registers */
		RFM69_set_configuration( NVM_info_s.NVM_generic_data_blk_s.tx_power_level );

		RFM69_set_PA_level( RFM69_tx_power_level_s );

		RFM69_set_own_node_address( NVM_info_s.NVM_generic_data_blk_s.node_id );

		RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGOPMODE, read_data, sizeof( read_data ) );

		/* Put the chip into sleep mode */
		RFM69_set_operating_mode( RFM69_SLEEP_MODE );

		/* this sets DIO1 to be "Packet Sent" indicator */
		RFM69_set_DIO_mapping( 0, RFM69_DIO_MODE_1 );
		RFM69_set_DIO_mapping( 1, RFM69_DIO_MODE_2 );
		RFM69_set_DIO_mapping( 2, RFM69_DIO_MODE_1 );
		RFM69_set_DIO_mapping( 3, RFM69_DIO_MODE_2 );
		RFM69_set_DIO_mapping( 4, RFM69_DIO_MODE_2 );
		RFM69_set_DIO_mapping( 5, RFM69_DIO_MODE_3 );
	}
	else
	{
		STDC_basic_assert();
	}

	RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGOPMODE, read_data, sizeof( read_data ) );

	RFM69_set_operating_mode( RFM69_RECEIVE_MODE );
}



/*!
****************************************************************************************************
*
*   \brief         Module (re-)initialisation function
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RFM69_receive_frame( void )
{
	u8_t read_data[RFM69_MAX_DATA_LEN];
	u8_t register_val = 0u;

	if( RFM69_packet_received_s == TRUE )
	{
		/* Reset the flag */
		RFM69_packet_received_s = FALSE;

		/* Grab the current value of RSSi as quickly as possible */
		//RFM69_last_rssi_s = RFM69_read_RSSI();

		/* Set to standby mode and read the FIFO for the RF data */
		RFM69_set_operating_mode( RFM69_STANDBY_MODE );
		RFM69_read_FIFO_register( read_data );
		RFM69_set_operating_mode( RFM69_RECEIVE_MODE );

        /* Increment the packet count */
        RFM69_received_packet_cnt_s ++;

        RFM69_timestamp_receieved_packet();
	}
}








/*!
****************************************************************************************************
*
*   \brief         Initialise all the RF registers
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
pass_fail_et RFM69_set_configuration( RFM69_static_configuration_et config )
{
    pass_fail_et returnType = PASS;

    if( config >= RFM69_CFG_MAX )
    {
        // invalid configuration
        STDC_basic_assert();
    }
    else
    {
        /* Config is OK */
    	u8_t len;
    	len = RFM69_config_c[ config ].length;

#if(MULTI_SPI_WRITE_CONFIG==0)

        u8_t i;

        for( i = 0u; i < len; i++ )
        {
            // write to all registers defined in lookup table for configuration to RFM69 chip
            if( RFM69_write_registers( WRITE_TO_CHIP, RFM69_config_c[config].buffer_p[i].RFM69_register, &RFM69_config_c[config].buffer_p[i].register_data, 1 ) == FAIL )
            {
                /* Configuration failed :( */
                STDC_basic_assert();

                returnType = FAIL;
            }
        }

#else
        if( RFM69_write_registers( WRITE_TO_CHIP_BURST_MODE_CONF, RFM69_config_c[config].buffer_p[0].RFM69_register, &RFM69_config_c[config].buffer_p[0].register_data, len  ) == FAIL )
		{
			/* Configuration failed :( */
			STDC_basic_assert();

			returnType = FAIL;
		}

		u8_t val = 0x30;

		/* This register sits out on its own but the datasheet says to set it to this value so i will comply with the law */
		if( RFM69_write_registers( WRITE_TO_CHIP, REGTESTDAGC, &val, 1 ) == FAIL )
        {
            /* Configuration failed :( */
			STDC_basic_assert();

			returnType = FAIL;
        }

#endif
    }

    return ( returnType );
}




/*!
****************************************************************************************************
*
*   \brief         Initialise all the RF registers
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RFM69_get_configuration( RFM69_static_configuration_et config, RFM69_register_data_st* data_p )
{
    u8_t i,j = 0;

    for(  i = 0; i < RFM69_config_c[ config ].length; i++ )
    {
    	data_p[i].RFM69_register = RFM69_config_c[config].buffer_p[i].RFM69_register;
    	data_p[i].register_data  = RFM69_config_c[config].buffer_p[i].register_data;

    	data_p->RFM69_register;
    }
}



/*!
****************************************************************************************************
*
*   \brief         Reads several bytes from the RF chip
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
pass_fail_et RFM69_read_registers( RFM69_instruction_et instruction, RFM69_registers_et address, u8_t read_data[], u8_t num_bytes )
{
    /* assume pass until prove otherwise */
    pass_fail_et result = PASS;
    u8_t i = 0;

    /* Need to make sure that the SPI interface is initialised */
    if( HAL_SPI_get_init_status() == FALSE )
    {
        /* it is not initialised so do that now */
    	HAL_SPI_init();
    }

    /* Instruction and address looks good so carry out the read */
    if( ( instruction != READ_FROM_CHIP ) && ( instruction != READ_FROM_CHIP_BURST_MODE ) )
    {
        result = FAIL;
    }
    else if( address > REG_MAX )
    {
        result = FAIL;
    }
    else
    {
        /* OK to continue */

        /* pull NCS / Slave Select line low */
        HAL_BRD_RFM69_spi_slave_select( LOW );

        /* Send the address */
        HAL_SPI_write_and_read_data( address );

        if( instruction == READ_FROM_CHIP )
        {
            /* Only 1 byte allowed to be Read for this instruction */
            num_bytes = 1;
        }

        /* Writes data to chip */
        for( i = 0; i < num_bytes; i++ )
        {
            /* send dummy byte to force readback */
            read_data[ i ] = (u8_t)HAL_SPI_write_and_read_data( 0xFF );
        }

        /* pull NCS / Slave Select line high */
        HAL_BRD_RFM69_spi_slave_select( HIGH );
    }

    return( result );
}




/*!
*******************************************************************************
*
*   \brief          Writes several bytes of consecutive data starting at a particular register
*
*   \author         MS
*
*   \param          config - configuration in question
*   \param          instruction - Instruction to be executed
*   \param          address - address to be written to
*   \param          write_data - data to be written
*   \param          num_bytes - Number of bytes to be written
*
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
pass_fail_et RFM69_write_registers( RFM69_instruction_et instruction, RFM69_registers_et address, u8_t write_data[], u8_t num_bytes )
{
    /* assume pass until prove otherwise */
    pass_fail_et result = PASS;
    u8_t i;
    u8_t modified_address;

    /* validate input parameters */
    if( ( instruction != WRITE_TO_CHIP ) && ( instruction != WRITE_TO_CHIP_BURST_MODE ) && ( instruction != WRITE_TO_CHIP_BURST_MODE_CONF ) )
    {
        result = FAIL;
        STDC_assert( 1 );
    }
    /* Write only registers start at 0, so only need to validate upper limit */
    else if( address >= REG_MAX )
    {
        result = FAIL;
        STDC_assert( 1 );
    }
    else
    {
        /* OK to continue */

        /* pull NCS / Slave Select line low */
        HAL_BRD_RFM69_spi_slave_select( LOW );

        /* Now we need to modify the address with the "write" bit set */
        modified_address = ( address | RFM69_WRITE_ACCESS );

        /* Send register address */
        HAL_SPI_write_and_read_data( modified_address );

        if( instruction == WRITE_TO_CHIP )
        {
            /* Only 1 byte allowed to be Read / Written for this instruction*/
            num_bytes = 1;
        }

        u16_t j = 0;

        /* Write all data to chip */
        for( i = 0; i < num_bytes; i++ )
        {
        	if(  instruction == WRITE_TO_CHIP_BURST_MODE_CONF )
        	{
        		HAL_SPI_write_and_read_data( write_data[ j ] );
				j += 2;
        	}
        	else
        	{
        		HAL_SPI_write_and_read_data( write_data[ i ] );
        	}
        }

        /* release NCS / Slave Select line high */
        HAL_BRD_RFM69_spi_slave_select( HIGH );

#ifdef READ_BACK_REGISTERS
        /* Now read back the register to ensure it was written succesfully,
        NOTE!! We can only read back the registers that we can write data to */
        u8_t read_back_register[num_bytes];

        RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, address, read_back_register, num_bytes );

        /* compare actual values with expected values */
        if( STDC_memcompare( read_back_register, write_data, num_bytes ) == FALSE )
        {
            result = FAIL;
            STDC_basic_assert();
        }
#endif
    }

    return( result );
}


/*!
*******************************************************************************
*
*   \brief          Reads the version number of the RFM69 chip
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
u8_t RFM69_read_version_num( void )
{
 	u8_t register_val;

 	RFM69_read_registers( READ_FROM_CHIP, REGVERSION, &register_val, 1 );

    return( register_val );
}


/*!
*******************************************************************************
*
*   \brief          Reads the current RF frequency of the chip
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
u32_t RFM69_read_rf_carrier_freq( void )
{
 	u32_t carrier_freq = 0u;
 	u8_t register_val;

 	RFM69_read_registers( READ_FROM_CHIP, REGFRFMSB, &register_val, 1 );
    carrier_freq |= ( register_val << 16 );

 	RFM69_read_registers( READ_FROM_CHIP, REGFRFMID, &register_val, 1 );
 	carrier_freq |= ( register_val << 8 );

 	RFM69_read_registers( READ_FROM_CHIP, REGFRFLSB, &register_val, 1 );
 	carrier_freq |= ( register_val );

 	/* Multiply by the freq step factor to get the carrier freq */
 	carrier_freq = ( carrier_freq * FREQ_STEP );

    return( carrier_freq );
}


/*!
*******************************************************************************
*
*   \brief          Reads the temperature the RFM69 chip
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
u8_t RFM69_read_temp( void )
{
 	u8_t register_val = 0u;
 	u16_t temp = 0u;

 	/* Read back the current register status */
	RFM69_read_registers( READ_FROM_CHIP, REGOPMODE, &register_val, 1 );

	if( register_val != RFM69_STANDBY_MODE )
    {
        /* NEED to be in standby mode before this can be done */
		RFM69_set_operating_mode( RFM69_STANDBY_MODE );
    }

	register_val = 0u;

	/* start the temperature measurement */
	register_val |= ( TEMP_MEASURE_START );
	RFM69_read_registers( WRITE_TO_CHIP, REGTEMP1, &register_val, 1 );

	register_val = 0;

	while ( ( register_val & ( TEMP_MEASURE_RUNNING ) ) == (  TEMP_MEASURE_RUNNING ) )
	{
		RFM69_read_registers( READ_FROM_CHIP, REGTEMP1, &register_val, 1 );
	}

	/* Read the temperature measurement */
	RFM69_read_registers( READ_FROM_CHIP, REGTEMP2, &temp, 1 );
	temp += COURSE_TEMP_COEF;
	temp ^= 0xFF;

    return( (u8_t)temp );
}






/*!
*******************************************************************************
*
*   \brief          Reads the RX RSSI
*
*   \author         MS
*
*   \return         result - rssi in dBm
*
*******************************************************************************
*/
u16_t RFM69_read_RSSI( void )
{
 	u8_t register_val;
 	s8_t rssi;

 	/* Trigger the RSSI measurement */
 	RFM69_trigger_RSSi_measurement();

 	/* Now Read back the current value in the RSSi register  */
	RFM69_read_registers( READ_FROM_CHIP, REGRSSIVALUE, &register_val, 1 );

	rssi = ( register_val / 2 );

	/* Make the number negative */
	rssi * -1;

    return( register_val );
}



u8_t RFM69_trigger_RSSi_measurement( void )
{
    u8_t register_val = 0u;
    false_true_et status = FALSE;

    /* Trigger the RSSI measurement */
    register_val |= ( RSSI_START );

    RFM69_write_registers( WRITE_TO_CHIP, REGRSSICONFIG, &register_val, 1 );

    register_val = 0u;

    /* Now wait for the RSSI read to be completed */
    while( ( register_val & RF_RSSI_DONE ) != RF_RSSI_DONE )
    {
    	RFM69_read_registers( READ_FROM_CHIP, REGRSSICONFIG, &register_val, 1 );
    }

    return ( status );
}



false_true_et RFM69_read_FIFO_register( u8_t* data_p )
{
    false_true_et status = FALSE;

    RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGFIFO, data_p, RFM69_MAX_DATA_LEN );
    status = TRUE;

    return ( status );
}




/*!
*******************************************************************************
*
*   \brief          Reads the Reserved Registers of the RFM69 chip
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
pass_fail_et RFM69_read_reserved_registers( void )
{
	pass_fail_et result = PASS;
 	u8_t reserved_vals[4] = { 0x40, 0xB0, 0x7B, 0x9B };
 	u8_t readback_vals[ sizeof( reserved_vals ) ];

 	RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, RESERVED14, readback_vals, sizeof( readback_vals ) );

	/* compare actual values with expected values */
	if( STDC_memcompare( reserved_vals, readback_vals, sizeof( reserved_vals ) ) == FALSE )
	{
		result = FAIL;
		STDC_basic_assert();
	}

    return( result );
}


/*!
*******************************************************************************
*
*   \brief          Turns on or off the CLK_OUT feature on DI05
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
false_true_et RFM69_set_clock_out( disable_enable_et state )
{
 	u8_t register_val;

 	/* Read back the current register status, modify it and then rewrite it back down */
	RFM69_read_registers( READ_FROM_CHIP, REGDIOMAPPING2, &register_val, 1 );

    /* This masks off and enables the CLK out */
    register_val &= ~BIT_MASK_3_BIT;

    /* This disables the CLK_OUT */
 	if( state == DISABLE_ )
    {
        register_val |= CLK_OUT_OFF;
    }

    RFM69_write_registers( WRITE_TO_CHIP, REGDIOMAPPING2, &register_val, 1 );

    return( PASS );
}



/*!
*******************************************************************************
*
*   \brief          Sets the Transmitter power level
*
*   \author         MS
*
*   \return         u8_t power level
*
*******************************************************************************
*/
u8_t RFM69_set_PA_level( u8_t level )
{
 	u8_t register_val = 0;

#if defined RFM69W
    register_val |= 0x80;
#elif defined RFM69HW
    register_val |= 0x60;
#elif defined RFM69HW_SUPER
    register_val |= 0xE0;
#endif

    if( level > RFM69_MAX_TX_POWER_LEVEL )
    {
    	level = RFM69_MAX_TX_POWER_LEVEL;
    }

    /* "OR" in the requested level between 0 and 31 */
    register_val |= level;

	RFM69_tx_power_level_s = level;

    RFM69_write_registers( WRITE_TO_CHIP, REGPALEVEL, &register_val, 1 );

    /* Read back the current register status */
	RFM69_read_registers( READ_FROM_CHIP, REGPALEVEL, &register_val, 1 );

    return( level );
}




/*!
*******************************************************************************
*
*   \brief          Sets the operational mode of the RFM69
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
false_true_et RFM69_set_operating_mode( RFM69_operating_modes_et operating_mode )
{
 	u8_t register_val;
 	false_true_et listen_mode_enabled;
 	RFM69_packet_sent_s = FALSE;

 	/* Read back the current register status, modify it and then rewrite it back down */
	RFM69_read_registers( READ_FROM_CHIP, REGOPMODE, &register_val, 1 );

	listen_mode_enabled = ( false_true_et )( ( register_val & ( LISTENON ) ) >> 6 );

	/* Clear the current power level setting */
	register_val &= ~( SLEEP | STDBY | FS | TX | RX );

	switch ( operating_mode )
	{
        case RFM69_SLEEP_MODE:

            /* Turn off CLK out to save power */
            RFM69_set_clock_out( OFF );
            break;

        case RFM69_STANDBY_MODE:
            register_val |= ( STDBY );
            break;

        case RFM69_FS_MODE:
            register_val |= ( FS );
            register_val &= ~( LISTENON );
            break;

        case RFM69_TRANSMIT_MODE:
            register_val |= (  TX );
            register_val &= ~( LISTENON );
            break;

        case RFM69_RECEIVE_MODE:
            register_val |= (  RX );
            register_val &= ~( LISTENON );
            break;

        case RFM69_LISTEN_MODE:
            register_val |= ( STDBY );
            register_val |= ( LISTENON );
            break;

        default:
            break;
	}

	if( ( listen_mode_enabled == TRUE ) && ( operating_mode != RFM69_LISTEN_MODE ) )
    {
        register_val |= ( LISTEN_ABORT );
        RFM69_write_registers( WRITE_TO_CHIP, REGOPMODE, &register_val, 1 );
        register_val &= ~(LISTEN_ABORT );
    }

    RFM69_write_registers( WRITE_TO_CHIP, REGOPMODE, &register_val, 1 );

    /* This checks to make sure that we have waited long enough for the radio to be ready after
       changing modes */
    register_val = 0u;
    while( ( register_val & 0x80 ) != 0x80 )
    {
		RFM69_read_registers( READ_FROM_CHIP, REGIRQFLAGS1, &register_val, 1 );
    }

    return( PASS );
}


/*!
*******************************************************************************
*
*   \brief          Sets the operational mode of the RFM69
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
false_true_et RFM69_set_packet_mode( RFM69_packet_modes_et operating_mode )
{
 	u8_t register_val;

 	/* Read back the current register status, modify it and then rewrite it back down */
	RFM69_read_registers( READ_FROM_CHIP, REGPACKETCONFIG1, &register_val, 1 );

	/* Clear the current setting */
	register_val &~( PACKET_FORMAT );
	register_val |= ( operating_mode << 7 );

    RFM69_write_registers( WRITE_TO_CHIP, REGPACKETCONFIG1, &register_val, 1 );

    return( PASS );
}



/*!
*******************************************************************************
*
*   \brief          Sets the payload length of the RFM69 packet
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
false_true_et RFM69_set_rx_payload_length( u8_t num_bytes )
{
    u8_t register_val;

	/* Clear the current setting */
	register_val = num_bytes;

    RFM69_write_registers( WRITE_TO_CHIP, REGPAYLOADLENGTH, &register_val, 1 );

    return( PASS );
}



/*!
*******************************************************************************
*
*   \brief          Sets the modulation type of the RF data
*
*   \author         MS
*
*   \return         result - pass or fail
*
*******************************************************************************
*/
false_true_et RFM69_set_modulation_type( RFM69_modulation_type_et type )
{
    u8_t register_val;
    false_true_et status;

    if( type >= RFM69_RESERVED)
    {
        status = FAIL;
    }
    else
    {
        /* Read back the current register status, modify it and then rewrite it back down */
        RFM69_read_registers( READ_FROM_CHIP, REGDATAMODUL, &register_val, 1 );

        register_val &~( MODULATION_TYPE );
        register_val |= ( type << 3 );

        RFM69_write_registers( WRITE_TO_CHIP, REGDATAMODUL, &register_val, 1 );

        status = PASS;
    }

    return( status );
}



false_true_et RFM69_set_data_mode( RFM69_data_mode_et mode )
{
    u8_t register_val;
    false_true_et status;

    if( mode >= RFM69_MODE_MAX)
    {
        status = FAIL;
    }
    else
    {
        /* Read back the current register status, modify it and then rewrite it back down */
        RFM69_read_registers( READ_FROM_CHIP, REGDATAMODUL, &register_val, 1 );

        register_val &~( DATA_PROCESS_MODE );
        register_val |= ( mode << 5 );

        RFM69_write_registers( WRITE_TO_CHIP, REGDATAMODUL, &register_val, 1 );

        status = PASS;
    }

    return( status );
}




false_true_et RFM69_set_bit_rate( RFM69_predefined_bitrates_et bit_rate )
{
    false_true_et status = TRUE;
    u8_t register_val;
    RFM69_modulation_type_et modulation_type;
    u16_t data_rate_setting;

     /* Read back the current register status ( modulation Scheme )*/
    RFM69_read_registers( READ_FROM_CHIP, REGDATAMODUL, &modulation_type, 1 );
    modulation_type = ( ( modulation_type & ( MODULATION_TYPE ) ) >> 3 );

    /* OOK only supports up to 19.2Kbps */
    if( modulation_type == RFM69_OOK  )
    {
        if( bit_rate > RFM69_19200bps )
        {
            bit_rate = RFM69_19200bps;
        }
    }

    switch( bit_rate )
    {
        /*! These are predeined standard values for bitrates taken from datasheet */
        case RFM69_1200bps:   data_rate_setting = 0x682B; break;
        case RFM69_2400bps:   data_rate_setting = 0x3415; break;
        case RFM69_4800bps:   data_rate_setting = 0x1A0B; break;
        case RFM69_9600bps:   data_rate_setting = 0x0D05; break;
        case RFM69_19200bps:  data_rate_setting = 0x0683; break;
        case RFM69_38400bps:  data_rate_setting = 0x0341; break;
        case RFM69_76800bps:  data_rate_setting = 0x01A1; break;
        case RFM69_153600bps: data_rate_setting = 0x00D0; break;
        default: break;
    }

    //! Write the MSB
    register_val = ( ( data_rate_setting & 0xFF00 ) >> 8 );
    RFM69_write_registers( WRITE_TO_CHIP, REGBITRATEMSB, &register_val, 1 );

    //! Write the LSB
    register_val = ( data_rate_setting & 0x00FF );
    RFM69_write_registers( WRITE_TO_CHIP, REGBITRATELSB, &register_val, 1 );

    return ( status );
}




false_true_et RFM69_set_listen_time_resolution( RFM69_listen_state_et state, RFM69_listen_time_et listen_time_resolution )
{
    u8_t register_val;
    false_true_et status = FALSE;

    //! Now check that the value is correct
    if( listen_time_resolution <= RFM_69_26200us )
    {
        register_val = listen_time_resolution;

        /* Read back the current register status, modify it and then rewrite it back down */
        RFM69_read_registers( READ_FROM_CHIP, REGLISTEN1, &register_val, 1 );

        //! Check that the state is correct
        if( state == RFM69_LISTEN_RX )
        {
             register_val &~( LISTEN_RX_RES );
             register_val |= ( listen_time_resolution << 5 );
             RFM69_write_registers( WRITE_TO_CHIP, REGLISTEN1, &register_val, 1 );

             status = TRUE;
        }
        else if ( state == RFM69_LISTEN_IDLE )
        {
             register_val & ~( LISTEN_IDLE_RES );
             register_val |= ( listen_time_resolution << 5 );
             RFM69_write_registers( WRITE_TO_CHIP, REGLISTEN1, &register_val, 1 );

             status = TRUE;
        }
        else
        {
        }
    }

    return ( status );
}



false_true_et RFM69_set_listen_time( RFM69_listen_state_et state, u16_t time )
{
    u8_t register_val;
    false_true_et status = FALSE;

    register_val = time;

    //! Check that the state is correct
    if( state == RFM69_LISTEN_RX )
    {
         RFM69_write_registers( WRITE_TO_CHIP, REGLISTEN2, &register_val, 1 );

         status = TRUE;
    }
    else if ( state == RFM69_LISTEN_IDLE )
    {
         RFM69_write_registers( WRITE_TO_CHIP, REGLISTEN3, &register_val, 1 );

         status = TRUE;
    }
    else
    {
    }

    return ( status );
}



false_true_et RFM69_set_encryption_key( u8_t* key, false_true_et state )
{
    u8_t register_val;
    false_true_et status = FALSE;

    /* Write down the encryption key */
    RFM69_write_registers( WRITE_TO_CHIP_BURST_MODE, REGAESKEY1, key, ENCRYPT_KEY_SIZE  );

    /* Read back the current register status, modify it and then rewrite it back down */
    RFM69_read_registers( READ_FROM_CHIP, REGPACKETCONFIG2, &register_val, 1 );

    /* Mask off the correct bits and turn encryption on or off */
    register_val &~( AES_ON ) ;
    register_val |= ( state ) ;
    RFM69_write_registers( WRITE_TO_CHIP, REGPACKETCONFIG2, &register_val, 1 );

    status = TRUE;

    return ( status );
}


false_true_et RFM69_set_own_node_address( u8_t address )
{
	u8_t register_val;
	false_true_et status = TRUE;

	/* Write down the node address */
	RFM69_write_registers( WRITE_TO_CHIP, REGNODEADRS, &register_val, 1  );

	return ( status );
}


false_true_et RFM69_set_own_network_id( u8_t network_id )
{
	u8_t register_val;
	false_true_et status = TRUE;

	register_val = network_id;

	/* Write down the encryption key */
	RFM69_write_registers( WRITE_TO_CHIP, REGSYNCVALU2, &register_val, 1  );

	return ( status );
}




false_true_et RFM69_set_DIO_mapping( u8_t pin, RFM69_DIO_map_mode_et mode )
{
    u8_t register_val;
    false_true_et status = FALSE;

    //!pins 0 - 3 are on one register and pins 4 - 5 on another
    if( pin <= 3 )
    {
        /* Read back the current register status, modify it and then rewrite it back down */
        RFM69_read_registers( READ_FROM_CHIP, REGDIOMAPPING1, &register_val, 1 );

        switch( pin )
        {
            case 0:
                register_val &= ~( DIO0 );
                register_val |= ( mode << 6 );
                break;
            case 1:
                register_val &= ~( DIO1 );
                register_val |= ( mode << 4 );
                break;
            case 2:
                register_val &= ~( DIO2 );
                register_val |= ( mode << 2 );
                break;
            case 3:
                register_val &= ~( DIO3 );
                register_val |= ( mode );
                break;

                default:
                    break;
        }

        RFM69_write_registers( WRITE_TO_CHIP, REGDIOMAPPING1, &register_val, 1 );
    }
    else
    {
        /* Read back the current register status, modify it and then rewrite it back down */
        RFM69_read_registers( READ_FROM_CHIP, REGDIOMAPPING2, &register_val, 1 );

        switch( pin )
        {
            case 4:
                register_val &= ~( DIO4 );
                register_val |= ( mode << 6 );
                break;
            case 5:
                register_val &= ~( DIO5 );
                register_val |=  ( mode << 4 );
                break;

            default:
                break;
        }

        RFM69_write_registers( WRITE_TO_CHIP, REGDIOMAPPING2, &register_val, 1 );
    }

    return ( status );
}





false_true_et RFM69_write_to_FIFO( u8_t* buffer, u8_t len )
{
    false_true_et status = FALSE;

    RFM69_write_registers( WRITE_TO_CHIP_BURST_MODE, REGFIFO, buffer, len );

    return ( status );
}



false_true_et RFM69_send_frame( u8_t* buffer, u8_t len, u8_t rx_node_address )
{
    false_true_et status = FALSE;
    u8_t tx_buffer[RFM69_MAX_PAYLOAD_LEN];
	u16_t timeout = 0u;
    u8_t test_buffer[RFM69_MAX_DATA_LEN];

    /* Set to standby */
    RFM69_set_operating_mode( RFM69_STANDBY_MODE );

    if( len > RFM69_MAX_PAYLOAD_LEN )
    {
    	len = RFM69_MAX_PAYLOAD_LEN;
    }

    tx_buffer[0] = len;

    STDC_memcpy( &tx_buffer[1], buffer, len );

    RFM69_write_to_FIFO( tx_buffer, len );

    //RFM69_read_FIFO_register( test_buffer );

    /* Set to TX mode */
    RFM69_set_operating_mode( RFM69_TRANSMIT_MODE );

    /* The packet is now being sent */
    while( ( RFM69_packet_sent_s == FALSE ) && ( timeout < RFM69_TIMEOUT ) )
    {
    	/* WE NEED TO HAVE A TIMEOUT HERE JUST SO THE WHOLE OPERATION DOESNT STOP BECAUSE
    	A FRAME DIDNT GET SENT */
    	delay_us( 100 );

    	/* increment the timeout */
    	timeout ++;
    }

    if( timeout == RFM69_TIMEOUT )
    {
        STDC_basic_assert();
    }

    /* Set to standby again */
    RFM69_set_operating_mode( RFM69_STANDBY_MODE );

    return ( status );
}


void RFM69_update_packet_sent( false_true_et state )
{
	RFM69_packet_sent_s = state;
}



void RFM69_update_packet_received( false_true_et state )
{
	RFM69_packet_received_s = state;
}


u32_t RFM69_get_received_packet_cnt( void )
{
	return( RFM69_received_packet_cnt_s );
}



f32_t RFM69_get_packet_reception_percent( void )
{
	return( RFM69_packet_reception_percent_s );
}


s8_t RFM69_get_last_received_RSSI( void )
{
	return ( RFM69_last_rssi_s );
}

void RFM69_timestamp_receieved_packet( void )
{
    RFM69_packet_received_timestamp_s = HAL_TIM_get_time();
}

u32_t RFM69_get_receieved_packet_timestamp( void )
{
    return ( RFM69_packet_received_timestamp_s );
}


/*!
****************************************************************************************************
*
*   \brief         RF Module reset
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RFM69_reset( void )
{
	/* Reset the init status */
	RFM69_init_s = FALSE;
}




void RFM69_set_reset_pin_state( low_high_et state )
{
    HAL_BRD_RFM69_set_reset_pin_state( state );
}


void RFM69_set_enable_pin_state( low_high_et state )
{
    HAL_BRD_RFM69_set_enable_pin_state( state );
}
