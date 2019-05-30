//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "STDC.h"


#include "STM32_stubs.h"


//-- module being tested
#include "MODE_MGR.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/



/*******************************************************************************
 *     Variables
 ******************************************************************************/
false_true_et 		    MODE_MGR_system_init_s;
u32_t 			        MODE_MGR_tick_timer_msecs_s;
MODE_MGR_mode_et        MODE_MGR_mode_s;
u8_t                    MODE_MGR_selector_switch_state_s;
u8_t                    MODE_MGR_debounce_ctr_s;


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
	MODE_MGR_init();

	TEST_ASSERT_EQUAL( MODE_MGR_TICK_RATE_MSECS, MODE_MGR_tick_timer_msecs_s );
	TEST_ASSERT_EQUAL( MODE_MGR_MODE_NORMAL, MODE_MGR_mode_s );
	TEST_ASSERT_EQUAL( FALSE, MODE_MGR_system_init_s );
	TEST_ASSERT_EQUAL( 0u, MODE_MGR_selector_switch_state_s );
	TEST_ASSERT_EQUAL( 0xFF, MODE_MGR_debounce_ctr_s );
}












void SERIAL_msg_handler( void ){}
void RFM69_receive_frame( void ){}
void NVM_tick( void ){}
void HAL_TIM_increment_secs( void ){}
void SERIAL_trigger_stream_output( void ){}
void HAL_BRD_toggle_debug_pin( void ){}
void NRF24_tick( void ){}
void CLI_message_handler( void ){}
void ROTARY_tick( void ){}
void HEATING_tick( void ){}
void MAIN_WATCHDOG_kick( void ){}
void RF_MGR_tick( void ){}
void NEOPIXEL_tick( void ){}
void RF_MGR_analyse_fault_conditions( void ){}
disable_enable_et HEATING_get_state( void ){}
void HEATING_set_mode( void ){}
void HEATING_get_mode( void ){}

void HEATING_set_state( disable_enable_et state )
{
	(void)state;
}

low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider )
{
	(void)slider;
	
	return( 0 );
}


