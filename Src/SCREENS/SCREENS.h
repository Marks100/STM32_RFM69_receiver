#ifndef SCREENS_H
#define SCREENS_H

#include "C_defs.h"
#include "LCD.h"
#include "HAL_BRD.h"


typedef enum
{
	WELCOME_MESSAGE_SCREEN = 0u,
	MAIN_MENU_SCREEN,
	INFO_SCREEN_1,
	INFO_SCREEN_2,
	INFO_SCREEN_3,
	TEST_SCREEN,
	EXPERT_MODE_SCREEN,
	AUTO_TARGET_TEMP_SCREEN
} SCREENS_screen_et;

typedef enum
{
    NO_REQUEST = 0u,
    INFO_REQUEST,
    EXPERT_MODE_REQUEST
} SCREENS_async_display_request_et;


#define DISPLAY_WELCOME_SCREEN_TIMEOUT  ( ( 3u * 1000 ) / LCD_TICK_RATE_MS )

void SCREENS_init();
void SCREENS_handle_rotary_input( ROTARY_scroll_type_et type );
void SCREENS_handle_screen( void );
void SCREENS_create_screen( SCREENS_screen_et screen );
void SCREENS_reset_screen_timer( void );
void SCREENS_set_async_requests( SCREENS_async_display_request_et request );

void SCREENS_create_welcome_screen( void );
void SCREENS_create_main_menu_screen( void );
void SCREENS_create_expert_mode_screen( void );
void SCREENS_create_info_screen_1( void );
void SCREENS_create_info_screen_2( void );
void SCREENS_create_info_screen_3( void );
void SCREENS_create_auto_temp_screen( void );

void SCREENS_create_test_screen( void );

u8_t SCREENS_handle_welcome_screen( void );
u8_t SCREENS_handle_main_menu_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_expert_mode_screen( void );
u8_t SCREENS_handle_info_screen_1( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_info_screen_2( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_info_screen_3( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_auto_temp_screen( ROTARY_scroll_type_et button_press );

u8_t SCREENS_handle_test_screen( ROTARY_scroll_type_et button_press );


#endif
