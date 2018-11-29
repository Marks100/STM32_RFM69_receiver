//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "STDC.h"


#include "STM32_stubs.h"


//-- module being tested
#include "MODE_MGR.h"

 typedef enum
{
    SLIDER_1 = 0u,
    SLIDER_2,
    SLIDER_3,
    SLIDER_4,
    SLIDER_MAX
} HAL_BRD_switch_slider_et;


/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/
void 		SERIAL_msg_handler( void );
void 		RFM69_receive_frame( void );
void 		NVM_tick( void );
void 		HAL_TIM_increment_secs( void );
void 		SERIAL_trigger_stream_output( void );
low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider );
void 		HAL_BRD_toggle_debug_pin( void );


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

	TEST_ASSERT_EQUAL( MODE_MGR_tick_timer_msecs_s, MODE_MGR_TICK_RATE_MSECS );
	TEST_ASSERT_EQUAL( MODE_MGR_mode_s, MODE_MGR_MODE_NORMAL );
	TEST_ASSERT_EQUAL( MODE_MGR_system_init_s, FALSE );
	TEST_ASSERT_EQUAL( MODE_MGR_selector_switch_state_s, 0u );
	TEST_ASSERT_EQUAL( MODE_MGR_debounce_ctr_s, 0xFF );
}








void SERIAL_msg_handler( void )
{
}

void RFM69_receive_frame( void )
{
}

void NVM_tick( void )
{
}

void HAL_TIM_increment_secs( void )
{
}

void SERIAL_trigger_stream_output( void )
{
}

low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider )
{
}

void HAL_BRD_toggle_debug_pin( void )
{
}





