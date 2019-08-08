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
extern AIRCON_config_st  AIRCON_config_s;
extern disable_enable_et AIRCON_generic_outputs_s[MAX_NUM_OUTPUTS];
extern AIRCON_min_max_st AIRCON_min_max_s;

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
    TEST_ASSERT_EQUAL( TMPERATURE_NOT_AVAILABLE, AIRCON_get_oat() );
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

    u8_t i, output_pin, loop;

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
    for( i = 0u; i < MAX_NUM_OUTPUTS; i++ )
    {
        TEST_ASSERT_EQUAL( DISABLE_, AIRCON_generic_outputs_s[i] );
    }

    for( loop = 0u; loop < MAX_NUM_OUTPUTS; loop++ )
    {
        output_pin = loop;
        AIRCON_togggle_generic_output_state( output_pin );
        for( i = 0; i < output_pin+1; i++ )
        {
            TEST_ASSERT_EQUAL( ENABLE_, AIRCON_generic_outputs_s[i] );
        }

        for( i = ( output_pin+1); i < MAX_NUM_OUTPUTS; i++ )
        {
            TEST_ASSERT_EQUAL( DISABLE_, AIRCON_generic_outputs_s[i] );
        }

        AIRCON_tick();
    }

    AIRCON_get_generic_output_state( 100 );
    AIRCON_tick();


    for( loop = 0u; loop < MAX_NUM_OUTPUTS; loop++ )
    {
        output_pin = loop;
        AIRCON_togggle_generic_output_state( output_pin );
        for( i = 0; i < output_pin+1; i++ )
        {
            TEST_ASSERT_EQUAL( DISABLE_, AIRCON_generic_outputs_s[i] );
        }

        for( i = ( output_pin+1); i < MAX_NUM_OUTPUTS; i++ )
        {
            TEST_ASSERT_EQUAL( ENABLE_, AIRCON_generic_outputs_s[i] );
        }

        AIRCON_tick();
    }

    AIRCON_get_generic_output_state( 100 );
    AIRCON_tick();
}

void test_decode_control_bytes( void )
{
    u8_t command = 0u;

    AIRCON_decode_control_cmd( command );
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

