#ifndef SCREENS_H
#define SCREENS_H

#include "C_defs.h"
#include "LCD.h"
#include "HAL_BRD.h"

#define SCREENS_MAIN_MEMU_ITEMS 9
#define SCREENS_INFO_MEMU_ITEMS 4
#define SCREENS_SET_TEMP_MEMU_ITEMS 4
#define SCREENS_SET_MIN_MAX_MEMU_ITEMS 3

typedef enum
{
	WELCOME_MESSAGE_SCREEN = 0u,
	MAIN_MENU_SCREEN,
	INFO_SCREEN_1,
	INFO_SCREEN_2,
	INFO_SCREEN_3,
	INFO_SCREEN_4,
	SET_TEMP_SEL_SCREEN,
	SET_AUTO_TEMP_SCREEN,
	SET_COOL_TEMP_SCREEN,
	SET_HEAT_TEMP_SCREEN,
	MONITOR_SCREEN,
	MODE_SELECT_SCREEN,
	STATE_SELECT_SCREEN,
	EXPERT_MODE_SCREEN,
	RESET_SCREEN,
	TEST_SCREEN
} SCREENS_screen_et;

typedef enum
{
    NO_REQUEST = 0u,
    INFO_REQUEST,
    EXPERT_MODE_REQUEST
} SCREENS_async_display_request_et;

typedef struct 
{
	u8_t  item_no;
	u8_t  screens[SCREENS_MAIN_MEMU_ITEMS];
	u8_t* list[SCREENS_MAIN_MEMU_ITEMS];
} SCREENS_main_memu_items_st;

typedef struct
{
	u8_t  item_no;
	u8_t  screens[SCREENS_INFO_MEMU_ITEMS];
} SCREENS_info_memu_items_st;

typedef struct
{
	u8_t  item_no;
	u8_t  screens[SCREENS_SET_TEMP_MEMU_ITEMS];
	u8_t* list[SCREENS_SET_TEMP_MEMU_ITEMS];
} SCREENS_set_temp_memu_items_st;

typedef struct
{
	u8_t  item_no;
	u8_t  screens[SCREENS_SET_MIN_MAX_MEMU_ITEMS];
	u8_t* list[SCREENS_SET_MIN_MAX_MEMU_ITEMS];
} SCREENS_set_temp_min_max_items_st;


#define DISPLAY_WELCOME_SCREEN_TIMEOUT  ( ( 3u * 1000u ) / LCD_TICK_RATE_MS )
#define DISPLAY_RESET_SCREEN_TIMEOUT    ( ( 2u * 1000u ) / LCD_TICK_RATE_MS )
#define DISPLAY_INACTIVITY_GENERAL_TIME ( ( 20u * 1000u ) / LCD_TICK_RATE_MS )

void SCREENS_init();
void SCREENS_handle_rotary_input( ROTARY_scroll_type_et type );
void SCREENS_handle_screen( void );
void SCREENS_create_screen( SCREENS_screen_et screen );
void SCREENS_reset_screen_timer( void );
void SCREENS_set_async_requests( SCREENS_async_display_request_et request );
void SCREENS_handle_inactivity_timer( void );
void SCREENS_reset_inactivity_timer( void );

void SCREENS_create_welcome_screen( void );
void SCREENS_create_main_menu_screen( void );
void SCREENS_create_expert_mode_screen( void );
void SCREENS_create_info_screen_1( void );
void SCREENS_create_info_screen_2( void );
void SCREENS_create_info_screen_3( void );
void SCREENS_create_info_screen_4( void );
void SCREENS_create_temp_sel_screen( void );
void SCREENS_create_auto_temp_screen( void );
void SCREENS_create_cool_temp_screen( void );
void SCREENS_create_heat_temp_screen( void );
void SCREENS_create_monitor_screen( void );
void SCREENS_create_reset_screen( void );
void SCREENS_create_mode_selector_screen( void );
void SCREENS_create_state_selector_screen( void );
void SCREENS_create_test_screen( void );

u8_t SCREENS_handle_welcome_screen( void );
u8_t SCREENS_handle_main_menu_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_expert_mode_screen( void );
u8_t SCREENS_handle_info_screens( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_auto_temp_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_temp_sel_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_heat_temp_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_cool_temp_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_monitor_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_reset_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_mode_selector_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_state_selector_screen( ROTARY_scroll_type_et button_press );
u8_t SCREENS_handle_test_screen( ROTARY_scroll_type_et button_press );


#endif
