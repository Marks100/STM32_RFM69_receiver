#ifndef SCREENS_H
#define SCREENS_H

#include "C_defs.h"
#include "LCD.h"


#define LCD_DISPLAY_WELCOME_SCREEN_TIMEOUT  ( ( 2u * 1000 ) / LCD_TICK_RATE_MS )
#define LCD_WELDER_FIRING_SCREEN_TIMEOUT_MS  ( 1000u / LCD_TICK_RATE_MS )
#define LCD_WELDER_HOT_WARNING_TIMEOUT_MS   ( 2000u / LCD_TICK_RATE_MS )



void Screens_init();
void LCD_reset_screen_timer( void );


void LCD_create_welcome_screen( void );
void LCD_create_main_menu_screen( void );
void LCD_create_safety_message_1_screen( void );
void LCD_create_safety_message_2_screen( void );
void LCD_create_firing_screen( void );
void LCD_create_expert_mode_screen( void );
void LCD_create_duty_cycle_warning_screen( void );
void LCD_create_info_screen_1( void );
void LCD_create_info_screen_2( void );
void LCD_create_info_screen_3( void );

u8_t LCD_handle_welcome_screen( void );
u8_t LCD_handle_main_menu_screen( false_true_et button_press );
u8_t LCD_handle_safety_message_1_screen( void );
u8_t LCD_handle_safety_message_2_screen( void );
u8_t LCD_handle_firing_screen( void );
u8_t LCD_handle_expert_mode_screen( void );
u8_t LCD_handle_duty_cycle_warning_screen( void );
u8_t LCD_handle_info_screen_1( false_true_et button_press );
u8_t LCD_handle_info_screen_2( false_true_et button_press );
u8_t LCD_handle_info_screen_3( false_true_et button_press );




#endif
