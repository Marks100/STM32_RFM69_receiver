//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "STDC.h"
#include "CHKSUM.h"
#include "NVM.h"

#include "STM32_stubs.h"


//-- module being tested
#include "NRF24.h"




/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/



/*******************************************************************************
 *     Variables
 ******************************************************************************/
const u8_t NRF24_data_pipe_default[5];
const u8_t NRF24_data_pipe_custom[5];

NRF24_state_et NRF24_state_s = NRF24_POWERING_UP;

u8_t  NRF24_rx_rf_payload_s[20];
u8_t  NRF24_status_register_s;
u8_t  NRF24_fifo_status_s;
u16_t NRF24_cycle_counter_s;
u8_t  NRF24_tx_rf_payload_s[3][32];


/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/
u8_t HAL_SPI_write_and_read_data( u8_t tx_data );
false_true_et HAL_SPI_get_init_status( void );
void HAL_SPI_init( void );
void HAL_BRD_NRF24_spi_slave_select( low_high_et state );
void HAL_BRD_NRF24_set_ce_pin_state( low_high_et state );


u8_t address_index_s = 0u;
u8_t register_data_s = 0u;
u8_t return_register_byte = 0xFF;


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
	NRF24_init();
}


void test_scheduler(void)
{
    NRF24_tick();
    NRF24_tick();
    NRF24_tick();
    NRF24_tick();
    NRF24_tick();
    NRF24_tick();
}


void test_mode( void )
{
    NRF24_set_low_level_mode( NRF_POWER_DOWN_MODE );
    return_register_byte = 0x00;
    NRF24_set_low_level_mode( NRF_POWER_DOWN_MODE );

    NRF24_set_low_level_mode( NRF_STANDBY_1_MODE );
    return_register_byte = 0x00;
    NRF24_set_low_level_mode( NRF_STANDBY_1_MODE );

    NRF24_set_low_level_mode( NRF_STANDBY_2_MODE );
    return_register_byte = 0xFF;
    NRF24_set_low_level_mode( NRF_STANDBY_2_MODE );

    NRF24_set_low_level_mode( NRF_RX_MODE );
    return_register_byte = 0x00;
    NRF24_set_low_level_mode( NRF_RX_MODE );

    NRF24_set_low_level_mode( NRF_TX_MODE );
    return_register_byte = 0xFF;
    NRF24_set_low_level_mode( NRF_TX_MODE );
}


void test_channel( void )
{
    return_register_byte = 0x00;
    NRF24_set_channel( 0 );
    NRF24_set_channel( 50 );
    NRF24_set_channel( 100 );
    NRF24_set_channel( 125 );
    NRF24_set_channel( 130 );

    return_register_byte = 0xFF;
    NRF24_set_channel( 0 );
    NRF24_set_channel( 50 );
    NRF24_set_channel( 100 );
    NRF24_set_channel( 125 );
    NRF24_set_channel( 130 );
}

void test_data_rate( void )
{
    return_register_byte = 0xFF;
    NRF24_set_rf_data_rate( RF24_250KBPS );
    NRF24_set_rf_data_rate( RF24_1MBPS );
    NRF24_set_rf_data_rate( RF24_2MBPS );

    return_register_byte = 0x00;
    NRF24_set_rf_data_rate( RF24_250KBPS );
    NRF24_set_rf_data_rate( RF24_1MBPS );
    NRF24_set_rf_data_rate( RF24_2MBPS );
}


void test_TX_PA_level( void )
{
    return_register_byte = 0xFF;
    NRF24_set_PA_TX_power( RF_MIN_TX_PWR );
    NRF24_set_PA_TX_power( RF_MED_1_TX_PWR );
    NRF24_set_PA_TX_power( RF_MED_2_TX_PWR );
    NRF24_set_PA_TX_power( RF_MAX_TX_PWR);

    return_register_byte = 0x00;
    NRF24_set_PA_TX_power( RF_MIN_TX_PWR );
    NRF24_set_PA_TX_power( RF_MED_1_TX_PWR );
    NRF24_set_PA_TX_power( RF_MED_2_TX_PWR );
    NRF24_set_PA_TX_power( RF_MAX_TX_PWR);
}


void test_crc_scheme( void )
{
    return_register_byte = 0x00;
    NRF24_setup_CRC_scheme( ENABLE, RF24_CRC_8 );
    NRF24_setup_CRC_scheme( ENABLE, RF24_CRC_16 );

    NRF24_setup_CRC_scheme( DISABLE, RF24_CRC_8 );
    NRF24_setup_CRC_scheme( DISABLE, RF24_CRC_16 );

    return_register_byte = 0xFF;
    NRF24_setup_CRC_scheme( ENABLE, RF24_CRC_8 );
    NRF24_setup_CRC_scheme( ENABLE, RF24_CRC_16 );

    NRF24_setup_CRC_scheme( DISABLE, RF24_CRC_8 );
    NRF24_setup_CRC_scheme( DISABLE, RF24_CRC_16 );
}


void test_cont_wave( void )
{
    return_register_byte = 0x00;
    NRF24_setup_constant_wave( ENABLE );
    NRF24_setup_constant_wave( DISABLE);

    return_register_byte = 0xFF;
    NRF24_setup_constant_wave( ENABLE );
    NRF24_setup_constant_wave( DISABLE);
}


void test_pll( void )
{
    return_register_byte = 0x00;
    NRF24_setup_pll( ENABLE );
    NRF24_setup_pll( DISABLE);

    return_register_byte = 0xFF;
    NRF24_setup_pll( ENABLE );
    NRF24_setup_pll( DISABLE);
}


void test_AA_pipe( void )
{
    return_register_byte = 0xFF;
    NRF24_set_AA_data_pipe( ENABLE, 0 );
    NRF24_set_AA_data_pipe( ENABLE, 1 );
    NRF24_set_AA_data_pipe( ENABLE, 2 );
    NRF24_set_AA_data_pipe( ENABLE, 3 );
    NRF24_set_AA_data_pipe( ENABLE, 4 );
    NRF24_set_AA_data_pipe( ENABLE, 5 );

    NRF24_set_AA_data_pipe( DISABLE, 0 );
    NRF24_set_AA_data_pipe( DISABLE, 1 );
    NRF24_set_AA_data_pipe( DISABLE, 2 );
    NRF24_set_AA_data_pipe( DISABLE, 3 );
    NRF24_set_AA_data_pipe( DISABLE, 4 );
    NRF24_set_AA_data_pipe( DISABLE, 5 );

    return_register_byte = 0x00;
    NRF24_set_AA_data_pipe( ENABLE, 0 );
    NRF24_set_AA_data_pipe( ENABLE, 1 );
    NRF24_set_AA_data_pipe( ENABLE, 2 );
    NRF24_set_AA_data_pipe( ENABLE, 3 );
    NRF24_set_AA_data_pipe( ENABLE, 4 );
    NRF24_set_AA_data_pipe( ENABLE, 5 );

    NRF24_set_AA_data_pipe( DISABLE, 0 );
    NRF24_set_AA_data_pipe( DISABLE, 1 );
    NRF24_set_AA_data_pipe( DISABLE, 2 );
    NRF24_set_AA_data_pipe( DISABLE, 3 );
    NRF24_set_AA_data_pipe( DISABLE, 4 );
    NRF24_set_AA_data_pipe( DISABLE, 5 );
}


void test_enable_pipe( void )
{
    return_register_byte = 0xFF;
    NRF24_set_AA_data_pipe( ENABLE, 0 );
    NRF24_set_AA_data_pipe( ENABLE, 1 );
    NRF24_set_AA_data_pipe( ENABLE, 2 );
    NRF24_set_AA_data_pipe( ENABLE, 3 );
    NRF24_set_AA_data_pipe( ENABLE, 4 );
    NRF24_set_AA_data_pipe( ENABLE, 5 );

    NRF24_set_AA_data_pipe( DISABLE, 0 );
    NRF24_set_AA_data_pipe( DISABLE, 1 );
    NRF24_set_AA_data_pipe( DISABLE, 2 );
    NRF24_set_AA_data_pipe( DISABLE, 3 );
    NRF24_set_AA_data_pipe( DISABLE, 4 );
    NRF24_set_AA_data_pipe( DISABLE, 5 );

    return_register_byte = 0x00;
    NRF24_set_AA_data_pipe( ENABLE, 0 );
    NRF24_set_AA_data_pipe( ENABLE, 1 );
    NRF24_set_AA_data_pipe( ENABLE, 2 );
    NRF24_set_AA_data_pipe( ENABLE, 3 );
    NRF24_set_AA_data_pipe( ENABLE, 4 );
    NRF24_set_AA_data_pipe( ENABLE, 5 );

    NRF24_set_AA_data_pipe( DISABLE, 0 );
    NRF24_set_AA_data_pipe( DISABLE, 1 );
    NRF24_set_AA_data_pipe( DISABLE, 2 );
    NRF24_set_AA_data_pipe( DISABLE, 3 );
    NRF24_set_AA_data_pipe( DISABLE, 4 );
    NRF24_set_AA_data_pipe( DISABLE, 5 );
}


void test_address_widths( void )
{
    return_register_byte = 0x00;
    NRF24_setup_address_widths( ADDRESS_3_BYTES );
    NRF24_setup_address_widths( ADDRESS_4_BYTES );
    NRF24_setup_address_widths( ADDRESS_5_BYTES );

    return_register_byte = 0xFF;
    NRF24_setup_address_widths( ADDRESS_3_BYTES );
    NRF24_setup_address_widths( ADDRESS_4_BYTES );
    NRF24_setup_address_widths( ADDRESS_5_BYTES );
}


void test_retries( void )
{
    return_register_byte = 0x00;
    NRF24_setup_retriese( RF24_250_US, 1 );
    NRF24_setup_retriese( RF24_750_US, 1 );
    NRF24_setup_retriese( RF24_4000_US, 1 );

    NRF24_setup_retriese( RF24_250_US, 5 );
    NRF24_setup_retriese( RF24_750_US, 5 );
    NRF24_setup_retriese( RF24_4000_US, 5 );

    return_register_byte = 0xFF;
    NRF24_setup_retriese( RF24_250_US, 15 );
    NRF24_setup_retriese( RF24_750_US, 15 );
    NRF24_setup_retriese( RF24_4000_US, 15 );

    NRF24_setup_retriese( RF24_250_US, 100 );
    NRF24_setup_retriese( RF24_750_US, 100 );
    NRF24_setup_retriese( RF24_4000_US, 105 );
}


void test_status_mask( void )
{
    u8_t data;
    low_high_et state;

    return_register_byte = 0xFF;
    state = NRF24_check_status_mask( RF24_RX_DATA_READY, &data );
    state = NRF24_check_status_mask( RF24_TX_DATA_SENT, &data );
    state = NRF24_check_status_mask( RF24_MAX_RETR_REACHED, &data );
    state = NRF24_check_status_mask( RF24_RX_PIPE_DATA_NUM, &data );
    state = NRF24_check_status_mask( RF24_TX_FIFO_FULL, &data );

    return_register_byte = 0x00;
    state = NRF24_check_status_mask( RF24_RX_DATA_READY, &data );
    state = NRF24_check_status_mask( RF24_TX_DATA_SENT, &data );
    state = NRF24_check_status_mask( RF24_MAX_RETR_REACHED, &data );
    state = NRF24_check_status_mask( RF24_RX_PIPE_DATA_NUM, &data );
    state = NRF24_check_status_mask( RF24_TX_FIFO_FULL, &data );
}

void test_dynamic_payloads( void )
{
    return_register_byte = 0x00;
    NRF24_set_dynamic_payloads( ENABLE, 0 );
    NRF24_set_dynamic_payloads( ENABLE, 3 );
    NRF24_set_dynamic_payloads( ENABLE, 5 );

    NRF24_set_dynamic_payloads( DISABLE, 0 );
    NRF24_set_dynamic_payloads( DISABLE, 3 );
    NRF24_set_dynamic_payloads( DISABLE, 5 );

    return_register_byte = 0xFF;
    NRF24_set_dynamic_payloads( ENABLE, 0 );
    NRF24_set_dynamic_payloads( ENABLE, 3 );
    NRF24_set_dynamic_payloads( ENABLE, 5 );

    NRF24_set_dynamic_payloads( DISABLE, 0 );
    NRF24_set_dynamic_payloads( DISABLE, 3 );
    NRF24_set_dynamic_payloads( DISABLE, 5 );
}



u8_t HAL_SPI_write_and_read_data( u8_t tx_data )
{
    register_data_s = tx_data;
    address_index_s ++;

    return ( return_register_byte );
}

false_true_et HAL_SPI_get_init_status( void )
{
    return ( TRUE );
}

void HAL_SPI_init( void )
{
    address_index_s = 0u;
    STDC_memset( register_data_s, 0x00, sizeof( register_data_s ) );
}

void HAL_BRD_NRF24_spi_slave_select( low_high_et state )
{
    /* Reset the address index */
    address_index_s = 0u;
}


void HAL_BRD_NRF24_set_ce_pin_state( low_high_et state )
{

}





