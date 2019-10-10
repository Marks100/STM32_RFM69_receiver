//-- unity: unit test framework
#include "unity.h"

#include "C_defs.h"
#include "Compiler_defs.h"
#include "STDC.h"
#include "STM32_stubs.h"


//-- module being tested
#include "TEMP.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/



/*******************************************************************************
 *     Variables
 ******************************************************************************/
float TEMP_temperature_c_s;
TEMP_state_et TEMP_state_s;
u16_t TEMP_timeout_s;
u16_t TEMP_fail_cnt_s;



pass_fail_et BMP280_self_check_status = PASS;
float BMP280_test_temp = S16_T_MAX;


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
	TEMP_init();

	TEST_ASSERT_FLOAT_WITHIN( 0.1, S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );
}



void test_cycling(void)
{
	TEMP_init();

	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_set_state( TEMP_SETUP );
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );
}

void test_fail_self_check_during_init( void )
{
	BMP280_self_check_status = FAIL;

	TEMP_init();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	BMP280_self_check_status = PASS;

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 3u, TEMP_fail_cnt_s );
}



void test_invalid_temps( void )
{
	BMP280_self_check_status = PASS;

	TEMP_init();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 2u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 3u, TEMP_fail_cnt_s );
}

void test_valid_temps_swing_to_invalid( void )
{
	BMP280_self_check_status = PASS;
	BMP280_test_temp = 20.5;
	float test_val;

	TEMP_init();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );
	TEST_ASSERT_EQUAL( FAIL, TEMP_get_last_temp_measurement(&test_val) );


	TEMP_cyclic();
	TEST_ASSERT_EQUAL( S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, S16_T_MAX, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, 20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, 20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );
	TEST_ASSERT_EQUAL( PASS, TEMP_get_last_temp_measurement(&test_val) );

	BMP280_test_temp = -20.5;

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -20.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	BMP280_test_temp = -100.5;
	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_ERROR, TEMP_state_s );
	TEST_ASSERT_EQUAL( 0u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_INIT, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_SETUP, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	BMP280_test_temp = 100.5;
	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, -100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, 100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, 100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );

	TEMP_cyclic();
	TEST_ASSERT_FLOAT_WITHIN( 0.1, 100.5, TEMP_temperature_c_s );
	TEST_ASSERT_EQUAL( TEMP_RUNNING, TEMP_state_s );
	TEST_ASSERT_EQUAL( 1u, TEMP_fail_cnt_s );
}







void BMP280_init( void )
{
} 

void BMP280_reset( void )
{
} 

void BMP280_setup_default_registers( void )
{
} 

pass_fail_et BMP280_self_check( void )
{
	return( BMP280_self_check_status );
} 

float BMP280_take_temp_measurement( void )
{
	return ( BMP280_test_temp );
}

void BMP280_read_calib_values( void )
{
}







