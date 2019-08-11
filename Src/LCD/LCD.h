#ifndef LCD_H
#define LCD_H

#include "C_defs.h"
#include "Screens.h"

#define LCD_TICK_RATE_MS    40u

#define LCD_COL_COUNT 16
#define LCD_ROW_COUNT 2

#define LCD_MAX_ROWS LCD_ROW_COUNT

// The rest should be left alone
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00

#define BIT7    7
#define BIT6    6
#define BIT5    5
#define BIT4    4
#define BIT3    3
#define BIT2    2
#define BIT1    1
#define BIT0    0


typedef enum
{
    LCD_DATA = 0u,
    LCD_COMMAND
} LCD_command_type_et;

typedef enum
{
    LCD_POWERING_UP  = 0,
    LCD_INITIALISING_P1,
    LCD_INITIALISING_P2,
    LCD_READY,
} LCD_state_et;


void LCD_set_D0_pin( low_high_et state );
void LCD_set_D1_pin( low_high_et state );
void LCD_set_D2_pin( low_high_et state );
void LCD_set_D3_pin( low_high_et state );
void LCD_set_D4_pin( low_high_et state );
void LCD_set_D5_pin( low_high_et state );
void LCD_set_D6_pin( low_high_et state );
void LCD_set_D7_pin( low_high_et state );
void LCD_set_RS_pin( low_high_et state );
void LCD_set_RW_pin( low_high_et state );
void LCD_set_EN_pin( low_high_et state );
void LCD_blip_EN_pin_high_low( void );
pass_fail_et LCD_read_busy_flag( void );
void LCD_set_cursor_position( u8_t row, u8_t colum );
void LCD_send_byte( u8_t byte, LCD_command_type_et type );
void LCD_send_command( u8_t command );
void LCD_init( void );
void LCD_clear(void);
void LCD_on(void);
void LCD_off( void );
void LCD_cursur_on( void );
void LCD_cursur_off( void );
void LCD_pos_blink_on( void );
void LCD_pos_blink_off( void );
void LCD_scroll_right( void );
void LCD_scroll_left( void );
false_true_et LCD_check_setting_enabled( u8_t settings_mask );
false_true_et LCD_write_message( u8_t* message, u8_t len );
void LCD_tick( void );




//void lcd_init(void);
//
//void lcd_command(uint8_t command);
//void lcd_write(uint8_t value);
//
//void lcd_on(void);
//void lcd_off(void);
//
//void lcd_clear(void);
//void lcd_return_home(void);
//
//void lcd_enable_blinking(void);
//void lcd_disable_blinking(void);
//
//void lcd_enable_cursor(void);
//void lcd_disable_cursor(void);
//
//void lcd_scroll_left(void);
//void lcd_scroll_right(void);
//
//void lcd_set_left_to_right(void);
//void lcd_set_right_to_left(void);
//
//void lcd_enable_autoscroll(void);
//void lcd_disable_autoscroll(void);
//
//void lcd_create_char(uint8_t location, uint8_t *charmap);
//
//void lcd_set_cursor(uint8_t col, uint8_t row);
//
//void lcd_puts(char *string);
//void lcd_printf(char *format, ...);


#endif
