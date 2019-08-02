//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "CHKSUM.h"
#include "STDC.h"
#include "NVM.h"

#include "STM32_stubs.h"

//-- module being tested
#include "AIRCON.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/
float HYSTERESIS = 0.5;


/*******************************************************************************
 *     Variables
 ******************************************************************************/

extern NVM_info_st NVM_info_s;
extern AIRCON_config_st   AIRCON_config_s;
extern disable_enable_et  AIRCON_generic_outputs_s[MAX_NUM_OUTPUTS];

typedef u8_t HAL_BRD_switch_slider_et;
typedef u8_t HAL_BRD_led_et;

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void)
{
    NVM_info_s.NVM_generic_data_blk_s.aircon_mode = DISABLE_;
    NVM_info_s.NVM_generic_data_blk_s.cool_max_temp = 19.0;
    NVM_info_s.NVM_generic_data_blk_s.cool_min_temp = 16.0;
    NVM_info_s.NVM_generic_data_blk_s.heat_max_temp = 21.0;
    NVM_info_s.NVM_generic_data_blk_s.heat_min_temp = 19.0;
    NVM_info_s.NVM_generic_data_blk_s.aircon_state = DISABLE_;
    NVM_info_s.NVM_generic_data_blk_s.auto_target_temp = 18.5;
    AIRCON_config_s.hysteresis = HYSTERESIS;

}

void tearDown(void)
{
}

/*******************************************************************************
 *    TESTS
 ******************************************************************************/


void test_init(void)
{
	AIRCON_init();

	TEST_ASSERT_EQUAL( 18.5, AIRCON_config_s.auto_target_temp_c );
    TEST_ASSERT_EQUAL( 19.0, AIRCON_config_s.cool_max_temp_c );
    TEST_ASSERT_EQUAL( 16.0, AIRCON_config_s.cool_min_temp_c );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( 21.0, AIRCON_config_s.heat_max_temp_c );
    TEST_ASSERT_EQUAL( 19.0, AIRCON_config_s.heat_min_temp_c );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
    TEST_ASSERT_EQUAL( 0.5, AIRCON_config_s.hysteresis );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.mode );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_config_s.oat_c );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.state );
	
    u8_t i;

    for( i = 0u; i < MAX_NUM_OUTPUTS; i++ )
    {
        TEST_ASSERT_EQUAL( LOW, AIRCON_generic_outputs_s[i]);
    }  
}


void test_outputs_are_disabled_when_system_off( void )
{
    AIRCON_config_s.state = DISABLE_;

    AIRCON_config_s.oat_c = 0.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 10.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 100.0;
    AIRCON_tick();
    AIRCON_tick();
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE, AIRCON_config_s.heater_state );
}

void test_heater_is_off_when_fan_is_on( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  AIRCON_COOL_MODE;

    AIRCON_config_s.oat_c = 0.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 10.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 100.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 18.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}

void test_fan_is_off_when_heater_is_on( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  AIRCON_HEAT_MODE;

    AIRCON_config_s.oat_c = 0.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 10.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 100.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 20.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}

void test_auto_mode_wide_swings_in_temp( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  AIRCON_AUTO_MODE;
    AIRCON_config_s.auto_target_temp_c = 20.0;

    AIRCON_config_s.oat_c = 0.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 10.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 100.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = -40.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.oat_c = 20.0;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}



void test_temp_invalid( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  AIRCON_AUTO_MODE;
    AIRCON_config_s.auto_target_temp_c = 20.0;

    AIRCON_set_oat(0);
    TEST_ASSERT_EQUAL(0, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );


    AIRCON_set_oat(-100);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(110);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );



    AIRCON_config_s.mode =  AIRCON_HEAT_MODE;
    AIRCON_set_oat(110);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(-110);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.mode =  AIRCON_COOL_MODE;
    AIRCON_set_oat(110);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(-55);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}




void NVM_tick( void ){}
void HAL_BRD_toggle_debug_pin( void ){}



low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider )
{
	(void)slider;
	
	return( 0 );
}

void HAL_BRD_set_LED_state( HAL_BRD_led_et led, low_high_et state )
{
    (void)led;
    (void)state;
}

void HAL_BRD_set_generic_output( u8_t generic_output_num, off_on_et state )
{
    (void)generic_output_num;
    (void)state;
}

