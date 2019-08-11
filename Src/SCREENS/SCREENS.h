#ifndef SCREENS_H
#define SCREENS_H

#include "C_defs.h"
#include "LCD.h"


typedef enum
{
	WELCOME_MESSAGE_SCREEN = 0u,
	SAFETY_INFO_P1_SCREEN,
	SAFETY_INFO_P2_SCREEN,
	MAIN_MENU_SCREEN,
	FIRING_SCREEN,
	DUTY_CYCLE_WARNING_SCREEN,
	INFO_SCREEN_1,
	INFO_SCREEN_2,
	INFO_SCREEN_3,
	EXPERT_MODE_SCREEN
} SCREENS_screen_et;

typedef enum
{
    NO_REQUEST = 0u,
    WELDER_FIRING_REQUEST,
    DUTY_CYCLE_WARNING_REQUEST,
    INFO_REQUEST,
    EXPERT_MODE_REQUEST
} SCREENS_async_display_request_et;


#define DISPLAY_WELCOME_SCREEN_TIMEOUT  ( ( 3u * 1000 ) / LCD_TICK_RATE_MS )

void SCREENS_init();
void SCREENS_handle_screen( void );
void SCREENS_create_screen( SCREENS_screen_et screen );
void SCREENS_reset_screen_timer( void );
void SCREENS_set_async_requests( SCREENS_async_display_request_et request );

void SCREENS_create_welcome_screen( void );
void SCREENS_create_main_menu_screen( void );
void SCREENS_create_safety_message_1_screen( void );
void SCREENS_create_safety_message_2_screen( void );
void SCREENS_create_firing_screen( void );
void SCREENS_create_expert_mode_screen( void );
void SCREENS_create_duty_cycle_warning_screen( void );
void SCREENS_create_info_screen_1( void );
void SCREENS_create_info_screen_2( void );
void SCREENS_create_info_screen_3( void );

u8_t SCREENS_handle_welcome_screen( void );
u8_t SCREENS_handle_main_menu_screen( false_true_et button_press );
u8_t SCREENS_handle_safety_message_1_screen( void );
u8_t SCREENS_handle_safety_message_2_screen( void );
u8_t SCREENS_handle_firing_screen( void );
u8_t SCREENS_handle_expert_mode_screen( void );
u8_t SCREENS_handle_duty_cycle_warning_screen( void );
u8_t SCREENS_handle_info_screen_1( false_true_et button_press );
u8_t SCREENS_handle_info_screen_2( false_true_et button_press );
u8_t SCREENS_handle_info_screen_3( false_true_et button_press );


#endif
