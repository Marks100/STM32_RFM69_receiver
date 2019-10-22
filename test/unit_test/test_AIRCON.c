//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "CHKSUM.h"
#include "STDC.h"
#include "NVM.h"

#include "STM32_stubs.h"
#include "mock_HAL_BRD.h" 

//-- module being tested
#include "AIRCON.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/


/*******************************************************************************
 *     Variables
 ******************************************************************************/

extern NVM_info_st       NVM_info_s;
extern AIRCON_config_st  AIRCON_config_s;
extern disable_enable_et AIRCON_generic_outputs_s[MAX_NUM_OUTPUTS];
extern AIRCON_min_max_st AIRCON_min_max_s;

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void)
{
    NVM_info_s.NVM_generic_data_blk_s.aircon_mode = DISABLE_;
    NVM_info_s.NVM_generic_data_blk_s.cool_target_temp = 19.0;
    NVM_info_s.NVM_generic_data_blk_s.heat_target_temp = 16.0;
    NVM_info_s.NVM_generic_data_blk_s.aircon_state = DISABLE_;
    NVM_info_s.NVM_generic_data_blk_s.auto_target_temp = 18.5;
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
    TEST_ASSERT_EQUAL( 19.0, AIRCON_config_s.cool_target_temp_c );
    TEST_ASSERT_EQUAL( 16.0, AIRCON_config_s.heat_target_temp_c );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
    TEST_ASSERT_EQUAL( 0.5, AIRCON_config_s.hysteresis );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.mode );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_get_oat() );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.state );
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
    AIRCON_config_s.oat_c = 20.0;

    AIRCON_set_oat(0, 0x0001);
    TEST_ASSERT_EQUAL(0, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(-100, 0x0001);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(110, 0x0001);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.mode =  AIRCON_HEAT_MODE;
    AIRCON_set_oat(110, 0x0001);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(-110, 0x0001);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.mode =  AIRCON_COOL_MODE;
    AIRCON_set_oat(110, 0x0001);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_set_oat(-55, 0x0001);
    TEST_ASSERT_EQUAL(TMPERATURE_INVALID, AIRCON_config_s.oat_c );
    AIRCON_tick();
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}

void test_temp_within_desired_range( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  AIRCON_HEAT_MODE;
    AIRCON_config_s.heat_target_temp_c = 20.0;
    AIRCON_config_s.cool_target_temp_c = 20.0;
    AIRCON_config_s.oat_c = 20.0;

    AIRCON_tick();
    TEST_ASSERT_EQUAL(20.0, AIRCON_config_s.oat_c );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_config_s.mode =  AIRCON_COOL_MODE;
    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}


void test_modes( void )
{
    AIRCON_config_s.oat_c = 20.0;
    AIRCON_set_mode(AIRCON_HEAT_MODE);
    TEST_ASSERT_EQUAL(AIRCON_HEAT_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_HEAT_MODE, AIRCON_config_s.mode );

    AIRCON_set_mode(AIRCON_COOL_MODE);
    TEST_ASSERT_EQUAL(AIRCON_COOL_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_COOL_MODE, AIRCON_config_s.mode );

    AIRCON_set_mode(AIRCON_AUTO_MODE);
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );

    AIRCON_set_mode(100);
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );


    AIRCON_set_mode(AIRCON_AUTO_MODE);
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );
    
    AIRCON_toggle_mode();  
    TEST_ASSERT_EQUAL(AIRCON_HEAT_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_HEAT_MODE, AIRCON_config_s.mode );

    AIRCON_toggle_mode();  
    TEST_ASSERT_EQUAL(AIRCON_COOL_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_COOL_MODE, AIRCON_config_s.mode );

    AIRCON_toggle_mode();  
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(AIRCON_AUTO_MODE, AIRCON_config_s.mode );
}


void test_state( void )
{
    AIRCON_config_s.state = DISABLE_;
    AIRCON_config_s.mode =  AIRCON_AUTO_MODE;
    AIRCON_config_s.auto_target_temp_c = 20.0;
    AIRCON_config_s.oat_c = 20.0;

    AIRCON_set_state( ENABLE_ );
    TEST_ASSERT_EQUAL(ENABLE_, AIRCON_get_state() );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(ENABLE_, AIRCON_get_state() );

    AIRCON_set_state( DISABLE_ );
    TEST_ASSERT_EQUAL(DISABLE_, AIRCON_get_state() );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(DISABLE_, AIRCON_get_state() );

    AIRCON_set_state( 100 );
    TEST_ASSERT_EQUAL(DISABLE_, AIRCON_get_state() );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(DISABLE_, AIRCON_get_state() );

    AIRCON_toggle_state();
    TEST_ASSERT_EQUAL(ENABLE_, AIRCON_get_state() );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(ENABLE_, AIRCON_get_state() );

    AIRCON_toggle_state();
    TEST_ASSERT_EQUAL(DISABLE_, AIRCON_get_state() );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(DISABLE_, AIRCON_get_state() );

    AIRCON_toggle_state();
    TEST_ASSERT_EQUAL(ENABLE_, AIRCON_get_state() );
    AIRCON_tick();
    TEST_ASSERT_EQUAL(ENABLE_, AIRCON_get_state() );
}

void test_outputs_default_case( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  100;
    AIRCON_config_s.auto_target_temp_c = 20.0;
    AIRCON_config_s.oat_c = 20.0;

    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );

    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
}


void test_generic_outputs( void )
{
    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  100;
    AIRCON_config_s.auto_target_temp_c = 20.0;
    AIRCON_config_s.oat_c = 20.0;

    u8_t i;

    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
    for( i = 0u; i < MAX_NUM_OUTPUTS; i++ )
    {
        TEST_ASSERT_EQUAL( DISABLE_, AIRCON_generic_outputs_s[i] );
    }


    AIRCON_tick();
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
 }


void test_min_max_tracking( void )
{
    AIRCON_init();
    float temperature_val ;

    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  100;
    AIRCON_config_s.auto_target_temp_c = 20.0;
    AIRCON_config_s.oat_c = 20.0;

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    AIRCON_tick();

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    temperature_val = -50.1;
    AIRCON_set_oat(temperature_val, 0x0001);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    temperature_val = 100.1;
    AIRCON_set_oat( temperature_val, 0x0001);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    temperature_val = 25.0;
    AIRCON_set_oat( temperature_val, 0x0001);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( temperature_val, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 1, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( temperature_val, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 1, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    temperature_val = 24.9;
    AIRCON_set_oat( temperature_val, 0x0002);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( 25.0, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 1, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( 24.9, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0.1, AIRCON_min_max_s.temp_delta_c );

    temperature_val = 27.0;
    AIRCON_set_oat( temperature_val, 0x0002);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( temperature_val, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( 24.9, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.temp_delta_c );

    temperature_val = 21.0;
    AIRCON_set_oat( temperature_val, 0x0003);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( 27.0, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( temperature_val, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 3, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 6, AIRCON_min_max_s.temp_delta_c );

    temperature_val = 13.0;
    AIRCON_set_oat( temperature_val, 0x0002);
    AIRCON_tick();

    TEST_ASSERT_EQUAL( 27.0, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( temperature_val, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 2, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 14, AIRCON_min_max_s.temp_delta_c );
}


void test_get_temp_setting( void )
{
    AIRCON_init();
    float temperature_val ;

    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  100;
    AIRCON_config_s.auto_target_temp_c = 20.0;
    AIRCON_config_s.cool_target_temp_c = 15.5;
    AIRCON_config_s.heat_target_temp_c = 18.5;
    AIRCON_config_s.oat_c = 20.0;

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    AIRCON_tick();

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    TEST_ASSERT_EQUAL( 18.5, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 15.5, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 20.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );
}


void test_AIRCON_adjust_temperature_setting_up( void )
{
    AIRCON_init();
    float temperature_val ;

    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  100;
    AIRCON_config_s.auto_target_temp_c = 5.0;
    AIRCON_config_s.cool_target_temp_c = 6.5;
    AIRCON_config_s.heat_target_temp_c = 7.5;
    AIRCON_config_s.oat_c = 20.0;

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    /* Auto temp settings up */
    TEST_ASSERT_EQUAL( 7.5, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 6.5, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 5.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING + STEP_VALUE, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING + ( STEP_VALUE * 2u ), AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    u8_t i = 0;
    for( i = 0u; i < 100; i++ )
    {
        AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_UP );
    }
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    /* heat temp settings up */
    AIRCON_config_s.auto_target_temp_c = 5.0;
    AIRCON_config_s.cool_target_temp_c = 6.5;
    AIRCON_config_s.heat_target_temp_c = 7.5;

    TEST_ASSERT_EQUAL( 7.5, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 6.5, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 5.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING + STEP_VALUE, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING + ( STEP_VALUE * 2u ), AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    for( i = 0u; i < 100; i++ )
    {
        AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_UP );
    }
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    /* cool temp settings up */
    AIRCON_config_s.auto_target_temp_c = 5.0;
    AIRCON_config_s.cool_target_temp_c = 6.5;
    AIRCON_config_s.heat_target_temp_c = 7.5;

    TEST_ASSERT_EQUAL( 7.5, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 6.5, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 5.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING + STEP_VALUE, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_UP );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING + ( STEP_VALUE * 2u ), AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    for( i = 0u; i < 100; i++ )
    {
        AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_UP );
    }
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );
}


void test_AIRCON_adjust_temperature_setting_down( void )
{
    AIRCON_init();
    float temperature_val ;

    AIRCON_config_s.state = ENABLE_;
    AIRCON_config_s.mode =  100;
    AIRCON_config_s.auto_target_temp_c = 100.0;
    AIRCON_config_s.cool_target_temp_c = 100.0;
    AIRCON_config_s.heat_target_temp_c = 100.0;
    AIRCON_config_s.oat_c = 40.0;

    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.max_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.max_temp_c.id );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_min_max_s.min_temp_c.value );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.min_temp_c.id );
    TEST_ASSERT_EQUAL( 0, AIRCON_min_max_s.temp_delta_c );

    /* Auto temp settings up */
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING - STEP_VALUE, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING - ( STEP_VALUE * 2u ), AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    u8_t i = 0;
    for( i = 0u; i < 100; i++ )
    {
        AIRCON_adjust_temperature_setting( AIRCON_AUTO_MODE, SETTING_ADJUST_DOWN );
    }
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    /* heat temp settings up */
    AIRCON_config_s.auto_target_temp_c = 100.0;
    AIRCON_config_s.cool_target_temp_c = 100.0;
    AIRCON_config_s.heat_target_temp_c = 100.0;

    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING - STEP_VALUE, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING - ( STEP_VALUE * 2u ), AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    for( i = 0u; i < 100; i++ )
    {
        AIRCON_adjust_temperature_setting( AIRCON_HEAT_MODE, SETTING_ADJUST_DOWN );
    }
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    /* cool temp settings up */
    AIRCON_config_s.auto_target_temp_c = 100.0;
    AIRCON_config_s.cool_target_temp_c = 100.0;
    AIRCON_config_s.heat_target_temp_c = 100.0;

    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_EQUAL( 100.0, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING - STEP_VALUE, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_DOWN );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING - ( STEP_VALUE * 2u ), AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );

    for( i = 0u; i < 100; i++ )
    {
        AIRCON_adjust_temperature_setting( AIRCON_COOL_MODE, SETTING_ADJUST_DOWN );
    }
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_HEAT_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MIN_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_COOL_MODE ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.1, MAX_AIRCON_TEMP_C_SETTING, AIRCON_get_temperature_setting( AIRCON_AUTO_MODE ) );
}



void test_AIRCON_get_heater_state( void )
{
    AIRCON_config_s.heater_state = ENABLE_;
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_get_heater_state() );

    AIRCON_config_s.heater_state = DISABLE_;
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_get_heater_state() );
}



void test_AIRCON_get_cooler_state( void )
{
    AIRCON_config_s.cooler_state = ENABLE_;
    TEST_ASSERT_EQUAL( ENABLE_, AIRCON_get_cooler_state() );

    AIRCON_config_s.cooler_state = DISABLE_;
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_get_cooler_state() );
}


void test_AIRCON_get_stats_address( void )
{
    AIRCON_get_stats_address();
}


void test_AIRCON_handle_rotary_input( void )
{
    ROTARY_scroll_type_et scroll_type = ROTARY_NO_CHANGE;
    AIRCON_handle_rotary_input( scroll_type );

    scroll_type = ROTARY_SHORT_PPRESS;
    AIRCON_handle_rotary_input( scroll_type );

    scroll_type = ROTARY_LONG_PPRESS;
    AIRCON_handle_rotary_input( scroll_type );
}


void test_get_temp_setting_fo_invalid_mode( void )
{
    test_init();

    AIRCON_config_s.mode = 100;
    AIRCON_get_temperature_setting( AIRCON_config_s.mode );

    TEST_ASSERT_EQUAL( 18.5, AIRCON_config_s.auto_target_temp_c );
    TEST_ASSERT_EQUAL( 19.0, AIRCON_config_s.cool_target_temp_c );
    TEST_ASSERT_EQUAL( 16.0, AIRCON_config_s.heat_target_temp_c );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.cooler_state );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.heater_state );
    TEST_ASSERT_EQUAL( 0.5, AIRCON_config_s.hysteresis );
    TEST_ASSERT_EQUAL( 100, AIRCON_config_s.mode );
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_get_oat() );
    TEST_ASSERT_EQUAL( DISABLE_, AIRCON_config_s.state );   
}



























void NVM_tick( void ){}
void HAL_BRD_toggle_debug_pin( void ){}

low_high_et HAL_BRD_read_selector_switch_pin( HAL_BRD_switch_slider_et slider )
{
	(void)slider;
	
	return( 0 );
}


void HAL_BRD_set_generic_output( u8_t generic_output_num, off_on_et state )
{
    (void)generic_output_num;
    (void)state;
}

