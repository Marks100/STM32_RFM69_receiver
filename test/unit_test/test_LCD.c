/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

//-- unity: unit test framework
#include "unity.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "C_defs.h"
#include "CHKSUM.h"
#include "COMPILER_defs.h"
#include "NVM.h"
#include "NVM_defaults.h"

#include "STDC.h"
#include "SELMATH.h"

#include "mock_HAL_BRD.h"
#include "mock_SCREENS.h"



//-- module being tested
#include "LCD.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/


/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/


/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/
extern u8_t         LCD_display_settings;
extern LCD_state_et LCD_state_s;

extern NVM_info_st NVM_info_s;



/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/


/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/
void setUp( void )
{
}





void tearDown( void )
{
}




/*******************************************************************************
 *    TESTS
 ******************************************************************************/
void test_init( void )
{
    LCD_init();

    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_POWERING_UP, LCD_state_s ); 
}

void test_LCD_full_initialisation( void )
{
    HAL_BRD_LCD_set_RS_pin_low_Ignore();
    HAL_BRD_74HC164_set_data_pin_low_Ignore();
    HAL_BRD_74HC164_pulse_clk_pin_state_Ignore();
    HAL_BRD_74HC164_set_data_pin_high_Ignore();
    HAL_BRD_LCD_pulse_enable_pin_state_Ignore();

    test_init();

    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_INITIALISING_P1, LCD_state_s ); 

    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_INITIALISING_P2, LCD_state_s ); 

    SCREENS_init_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 
}


void test_LCD_runnable( void )
{
    test_LCD_full_initialisation();

    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 

    SCREENS_handle_screen_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 

    SCREENS_handle_screen_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 

    SCREENS_handle_screen_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 
}


void test_user_selecting_system_reset( void )
{
    test_LCD_full_initialisation();

    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 

    SCREENS_handle_screen_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 

    SCREENS_handle_screen_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 

    SCREENS_handle_screen_Expect();
    LCD_tick();
    TEST_ASSERT_EQUAL( 0x00, LCD_display_settings );
    TEST_ASSERT_EQUAL( LCD_READY, LCD_state_s ); 
}




void delay_us( u16_t us )
{
    (void)us;
}
