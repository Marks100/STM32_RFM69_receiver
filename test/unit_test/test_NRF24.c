//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "STDC.h"


#include "STM32_stubs.h"


//-- module being tested
#include "NRF24.h"




/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/



/*******************************************************************************
 *     Variables
 ******************************************************************************/
const u8_t NRF24_data_pipe_defaut[5];
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
void HAL_BRD_NRF24_set_cs_pin_state( low_high_et state );




u8_t address_index_s = 0u;
u8_t register_data_s = 0u;


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




u8_t HAL_SPI_write_and_read_data( u8_t tx_data )
{
    register_data_s = tx_data;
    address_index_s ++;
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


void HAL_BRD_NRF24_set_cs_pin_state( low_high_et state )
{

}





