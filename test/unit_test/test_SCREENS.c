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
#include "mock_LCD.h"
#include "mock_AIRCON.h"

//-- module being tested
#include "SCREENS.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/


/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/

extern NVM_info_st NVM_info_s;

extern SCREENS_async_display_request_et SCREENS_async_display_request_s;
extern ROTARY_scroll_type_et            SCREENS_rotary_press_s;
extern u16_t                            SCREENS_screen_timer_s;
extern u32_t                            SCREENS_loop_ctr_s;
extern SCREENS_screen_et                SCREEN_screen_s;
extern u16_t                            SCREEN_inactivity_timer_s;


/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/

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
void test_init( void )
{
    SCREENS_init();

    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 0u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );
}

void test_welcome_screen( void )
{
    test_init();
    LCD_set_cursor_position_Ignore();
    LCD_write_message_IgnoreAndReturn(0);

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 1u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 2u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 3u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 4u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 5u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );

    u16_t time = ( DISPLAY_WELCOME_SCREEN_TIMEOUT );
    u16_t i;

    for( i = SCREENS_screen_timer_s+1; i < time; i++ )
    {
        SCREENS_handle_screen();
        TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
        TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
        TEST_ASSERT_EQUAL( i, SCREENS_screen_timer_s );
        TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );
    }

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 80u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( WELCOME_MESSAGE_SCREEN, SCREEN_screen_s );

    SCREENS_handle_screen();
    TEST_ASSERT_EQUAL( FALSE, SCREENS_async_display_request_s );
    TEST_ASSERT_EQUAL( ROTARY_NO_CHANGE, SCREENS_rotary_press_s );
    TEST_ASSERT_EQUAL( 0u, SCREENS_screen_timer_s );
    TEST_ASSERT_EQUAL( MONITOR_SCREEN, SCREEN_screen_s );
}


