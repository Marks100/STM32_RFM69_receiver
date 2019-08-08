#include "main.h"
#include "LCD.h"
#include "Screens.h"
#include "HAL_BRD.h"

STATIC u8_t LCD_display_settings;
STATIC LCD_screen_et LCD_screen_s;
STATIC LCD_async_display_request_et LCD_async_display_request_s;
STATIC LCD_button_press_s;
STATIC u8_t LCD_tick_ctr_s;



void LCD_init( void )
{
    LCD_display_settings = 0u;
    LCD_tick_ctr_s = 0u;

    /* Let the screen power up */
    //delay_ms( 20u );

    u8_t ctr;

    /* Need to do this initialisation 3 times */
    for ( ctr = 0u; ctr < 3; ctr++ )
    {
        LCD_send_command( LCD_FUNCTIONSET | LCD_8BITMODE );

        delay_ms(5);
    }

    /* Send down all the init commands */
    LCD_send_command( LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS );
    LCD_send_command( LCD_ENTRYMODESET | LCD_ENTRYLEFT );
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_DISPLAYON );

    LCD_screen_s = LCD_WELCOME_MESSAGE_SCREEN;
    LCD_async_display_request_s = LCD_NO_REQUEST;
    LCD_button_press_s = FALSE;

    /* Now initialise the actual screens themselves */
    Screens_init();


    /* We need to create the first screen ourself during the LCD init and then let the 100ms tick
    take over */
    LCD_create_screen( LCD_WELCOME_MESSAGE_SCREEN );
}



void LCD_set_RS_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_RS_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_RS_pin( LOW );
    }
}


void LCD_set_RW_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_RW_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_RW_pin( LOW );
    }
}


void LCD_set_EN_pin( low_high_et state )
{
    if( state == HIGH )
    {
    	HAL_BRD_set_EN_pin( HIGH );
    }
    else
    {
    	HAL_BRD_set_EN_pin( LOW );
    }
}


void LCD_blip_EN_pin_high_low( void )
{
	HAL_BRD_set_EN_pin( HIGH );
    delay_us( 2 );
    HAL_BRD_set_EN_pin( LOW );
}


void LCD_set_D0_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D0_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D0_pin( LOW );
    }
}


void LCD_set_D1_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D1_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D1_pin( LOW );
    }
}


void LCD_set_D2_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D2_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D2_pin( LOW );
    }
}


void LCD_set_D3_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D3_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D3_pin( LOW );
    }
}


void LCD_set_D4_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D4_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D4_pin( LOW );
    }
}


void LCD_set_D5_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D5_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D5_pin( LOW );
    }
}


void LCD_set_D6_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D6_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D6_pin( LOW );
    }
}


void LCD_set_D7_pin( low_high_et state )
{
    if( state == HIGH )
    {
        HAL_BRD_set_D7_pin( HIGH );
    }
    else
    {
        HAL_BRD_set_D7_pin( LOW );
    }
}


pass_fail_et LCD_read_busy_flag( void )
{
    low_high_et level = LOW;
    pass_fail_et status = FAIL;

    delay_us( 1 );

    /* First set RS LOW */
    LCD_set_RS_pin( LOW );

    /* Then set RW HIGH */
    LCD_set_RW_pin( HIGH );

    LCD_blip_EN_pin_high_low();

    /* Now configure D7 as an input and then read it */
    level == HAL_BRD_read_D7_pin();

    if( level == LOW )
    {
        status = PASS;
    }

    return ( status );
}



void LCD_set_cursor_position( u8_t row, u8_t colum )
{
    /*!
    ROW0 00 01 02 03 04 . . . . . . . . . . . . . . . . . . 26 27
    ROW1 40 41 42 43 44 . . . . . . . . . . . . . . . . . . 66 67
    ROW2 14 15 16 17 18 . . . . . . . . . . . . . . . . . . . . .
    ROW2 54 55 56 57 58 . . . . . . . . . . . . . . . . . . . . .
    */

    u8_t position_cmd = colum;

    u8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    if( row > LCD_MAX_ROWS )
    {
        row = LCD_MAX_ROWS;
    }

    position_cmd |= offsets[row];

    LCD_send_command( LCD_SETDDRAMADDR | position_cmd );
}



void LCD_set_data_on_pins ( u8_t data )
{
    /* D0 */
    LCD_set_D0_pin( (low_high_et)( ( data&0x01 ) >> BIT0 ) );

    /* D1 - D6  */
    LCD_set_D1_pin( (low_high_et)( ( data&0x02 ) >> BIT1 ) );
    LCD_set_D2_pin( (low_high_et)( ( data&0x04 ) >> BIT2 ) );
    LCD_set_D3_pin( (low_high_et)( ( data&0x08 ) >> BIT3 ) );
    LCD_set_D4_pin( (low_high_et)( ( data&0x10 ) >> BIT4 ) );
    LCD_set_D5_pin( (low_high_et)( ( data&0x20 ) >> BIT5 ) );
    LCD_set_D6_pin( (low_high_et)( ( data&0x40 ) >> BIT6 ) );

    /* D7 */
    LCD_set_D7_pin( (low_high_et)( ( data&0x80 ) >> BIT7 ) );
}


void LCD_clear_display( void )
{


}




void LCD_send_command( u8_t command )
{
    LCD_send_byte( command, LCD_COMMAND );
}



void LCD_send_data( u8_t data )
{
    LCD_send_byte( data, LCD_DATA );
}



void LCD_send_byte( u8_t byte, LCD_command_type_et type )
{
    pass_fail_et LCD_ready = FAIL;

    LCD_set_EN_pin( LOW );

    if( type == LCD_COMMAND )
    {
        LCD_set_RS_pin( LOW );
    }
    else
    {
        LCD_set_RS_pin( HIGH );
    }

    LCD_set_RW_pin( LOW );

    LCD_set_data_on_pins( byte );

    delay_us( 2 );

    LCD_blip_EN_pin_high_low();

#if( LCD_BUSY_FLAG_AVAILABLE == 1 )

    while( LCD_ready == FAIL )
    {
        LCD_ready = LCD_read_busy_flag();
    }
#else
    if( type == LCD_COMMAND  )
    {
    	delay_us(50);
    }
    else
    {
        delay_us(20);
    }

#endif
}


void LCD_clear( void )
{
    LCD_send_command( LCD_CLEARDISPLAY );
}

void LCD_return_home( void )
{
    LCD_send_command( LCD_RETURNHOME );
}


void LCD_on(void)
{
    LCD_display_settings |= LCD_DISPLAYON;
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_display_settings );
}


void LCD_off( void )
{
    LCD_display_settings &= ~LCD_DISPLAYON;
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_display_settings );
}

void LCD_cursur_on( void )
{
    LCD_display_settings |= LCD_DISPLAYON;
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_display_settings );
}

void LCD_cursur_off( void )
{
    LCD_display_settings &= ~LCD_CURSORON;
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_display_settings );
}


void LCD_pos_blink_on( void )
{
    LCD_display_settings |= LCD_BLINKON;
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_display_settings );
}


void LCD_pos_blink_off( void )
{
    LCD_display_settings &= ~LCD_BLINKON;
    LCD_send_command( LCD_DISPLAYCONTROL | LCD_display_settings );
}

void LCD_scroll_left( void )
{
    LCD_send_command( LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT );
}


void LCD_scroll_right( void )
{
    LCD_send_command( LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT );
}



false_true_et LCD_check_setting_enabled( u8_t settings_mask )
{
	false_true_et status = FALSE;

	if( ( LCD_display_settings & settings_mask ) == settings_mask )
	{
		status = TRUE;
	}

	return ( status );
}




false_true_et LCD_write_message( u8_t* message, u8_t size )
{
	false_true_et status = TRUE;
	u8_t i = 0;

	if( size > 16u )
    {
        /* The message wont fit on the screen, take appropriate action */
        status = FALSE;
    }
    else
    {
        for( i = 0u; i < size; i++ )
        {
            LCD_send_byte( *( message + i ), LCD_DATA );
        }
    }

	return ( status );
}




void LCD_200ms_tick( void )
{
    /* 100ms should be fine to update the screen */
    LCD_screen_s = LCD_handle_screen( LCD_screen_s );
    LCD_create_screen( LCD_screen_s );
}



LCD_screen_et LCD_handle_screen( LCD_screen_et screen )
{
    if( LCD_async_display_request_s != LCD_NO_REQUEST )
    {
        switch( LCD_async_display_request_s )
        {
            case LCD_WELDER_FIRING:
                screen = LCD_FIRING_SCREEN;
            break;

            case LCD_DUTY_CYCLE_WARNING:
                screen = LCD_DUTY_CYCLE_WARNING_SCREEN;
                break;

            case LCD_INFO:
                screen = LCD_INFO_SCREEN_1;
                break;

            case LCD_EXPERT_MODE:
                screen = LCD_EXPERT_MODE_SCREEN;
                break;

            default:
                break;
        }
        LCD_async_display_request_s = LCD_NO_REQUEST;
        LCD_reset_screen_timer();
    }

    switch( screen )
    {
        case LCD_WELCOME_MESSAGE_SCREEN:
           screen = LCD_handle_welcome_screen();
        break;

        case LCD_SAFETY_INFO_P1_SCREEN:
            screen = LCD_handle_safety_message_1_screen();
            break;

        case LCD_SAFETY_INFO_P2_SCREEN:
            screen = LCD_handle_safety_message_2_screen();
            break;

        case LCD_MAIN_MENU_SCREEN:
        	MODE_MGR_set_system_init_status( TRUE );
            screen = LCD_handle_main_menu_screen( LCD_button_press_s );
            break;

        case LCD_FIRING_SCREEN:
            screen = LCD_handle_firing_screen();
            break;

        case LCD_DUTY_CYCLE_WARNING_SCREEN:
            screen = LCD_handle_duty_cycle_warning_screen();
            break;

        case LCD_INFO_SCREEN_1:
            screen = LCD_handle_info_screen_1( LCD_button_press_s );
            break;

        case LCD_INFO_SCREEN_2:
            screen = LCD_handle_info_screen_2( LCD_button_press_s );
            break;

        case LCD_INFO_SCREEN_3:
            screen = LCD_handle_info_screen_3( LCD_button_press_s );
            break;

        case LCD_EXPERT_MODE_SCREEN:
            screen = LCD_handle_expert_mode_screen();
            break;

        default:
            break;
    }
    LCD_button_press_s = FALSE;

    return( screen );
}




void LCD_create_screen( LCD_screen_et screen )
{
    switch( screen )
    {
        case LCD_WELCOME_MESSAGE_SCREEN:
            LCD_create_welcome_screen();
            break;

        case LCD_SAFETY_INFO_P1_SCREEN:
            LCD_create_safety_message_1_screen();
            break;

        case LCD_SAFETY_INFO_P2_SCREEN:
            LCD_create_safety_message_2_screen();
            break;

        case LCD_MAIN_MENU_SCREEN:
            LCD_create_main_menu_screen();
            break;

        case LCD_FIRING_SCREEN:
            LCD_create_firing_screen();
            break;

        case LCD_DUTY_CYCLE_WARNING_SCREEN:
            LCD_create_duty_cycle_warning_screen();
            break;

        case LCD_INFO_SCREEN_1:
            LCD_create_info_screen_1();
            break;

        case LCD_INFO_SCREEN_2:
            LCD_create_info_screen_2();
            break;

        case LCD_INFO_SCREEN_3:
            LCD_create_info_screen_3();
            break;

        case LCD_EXPERT_MODE_SCREEN:
            LCD_create_expert_mode_screen();
            break;

        default:
        break;
    }
}



void LCD_set_async_requests( LCD_async_display_request_et request )
{
   LCD_async_display_request_s = request;
}



void LCD_set_button_press( false_true_et state )
{
    LCD_button_press_s = state;
}


false_true_et LCD_get_button_press( void )
{
    return ( LCD_button_press_s );
}









////
////
////void lcd_send(uint8_t value, uint8_t mode);
////void lcd_write_nibble(uint8_t nibble);
////
////static uint8_t lcd_displayparams;
////static char lcd_buffer[LCD_COL_COUNT + 1];
////
////void lcd_command(uint8_t command) {
////  lcd_send(command, 0);
////}
////
////void lcd_write(uint8_t value) {
////  lcd_send(value, 1);
////}
////
////void lcd_send(uint8_t value, uint8_t mode) {
////  if (mode) {
////    LCD_PORT = LCD_PORT | (1 << LCD_RS);
////  } else {
////    LCD_PORT = LCD_PORT & ~(1 << LCD_RS);
////  }
////
////  LCD_PORT = LCD_PORT & ~(1 << LCD_RW);
////
////  lcd_write_nibble(value >> 4);
////  lcd_write_nibble(value);
////}
////
////void lcd_write_nibble(uint8_t nibble) {
////  LCD_PORT = (LCD_PORT & 0xff & ~(0x0f << LCD_D0)) | ((nibble & 0x0f) << LCD_D0);
////
////  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
////  LCD_PORT = LCD_PORT | (1 << LCD_EN);
////  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
////  _delay_ms(0.04);
////}
////
////void lcd_init(void) {
////  // Configure pins as output
////  LCD_DDR = LCD_DDR
////    | (1 << LCD_RS)
////    | (1 << LCD_RW)
////    | (1 << LCD_EN)
////    | (1 << LCD_D0)
////    | (1 << LCD_D1)
////    | (1 << LCD_D2)
////    | (1 << LCD_D3);
////
////  // Wait for LCD to become ready (docs say 15ms+)
////  _delay_ms(15);
////
////  LCD_PORT = LCD_PORT
////    & ~(1 << LCD_EN)
////    & ~(1 << LCD_RS)
////    & ~(1 << LCD_RW);
////
////  _delay_ms(4.1);
////
////  lcd_write_nibble(0x03); // Switch to 4 bit mode
////  _delay_ms(4.1);
////
////  lcd_write_nibble(0x03); // 2nd time
////  _delay_ms(4.1);
////
////  lcd_write_nibble(0x03); // 3rd time
////  _delay_ms(4.1);
////
////  lcd_write_nibble(0x02); // Set 8-bit mode (?)
////
////  lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
////
////  lcd_displayparams = LCD_CURSOROFF | LCD_BLINKOFF;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_on(void) {
////  lcd_displayparams |= LCD_DISPLAYON;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_off(void) {
////  lcd_displayparams &= ~LCD_DISPLAYON;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_clear(void) {
////  lcd_command(LCD_CLEARDISPLAY);
////  _delay_ms(2);
////}
////
////void lcd_return_home(void) {
////  lcd_command(LCD_RETURNHOME);
////  _delay_ms(2);
////}
////
////void lcd_enable_blinking(void) {
////  lcd_displayparams |= LCD_BLINKON;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_disable_blinking(void) {
////  lcd_displayparams &= ~LCD_BLINKON;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_enable_cursor(void) {
////  lcd_displayparams |= LCD_CURSORON;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_disable_cursor(void) {
////  lcd_displayparams &= ~LCD_CURSORON;
////  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
////}
////
////void lcd_scroll_left(void) {
////  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
////}
////
////void lcd_scroll_right(void) {
////  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
////}
////
////void lcd_set_left_to_right(void) {
////  lcd_displayparams |= LCD_ENTRYLEFT;
////  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
////}
////
////void lcd_set_right_to_left(void) {
////  lcd_displayparams &= ~LCD_ENTRYLEFT;
////  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
////}
////
////void lcd_enable_autoscroll(void) {
////  lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
////  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
////}
////
////void lcd_disable_autoscroll(void) {
////  lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
////  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
////}
////
////void lcd_create_char(uint8_t location, uint8_t *charmap) {
////  lcd_command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
////  for (int i = 0; i < 8; i++) {
////    lcd_write(charmap[i]);
////  }
////}
////
////void lcd_set_cursor(uint8_t col, uint8_t row) {
////  static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };
////
////  if (row > 1) {
////    row = 1;
////  }
////
////  lcd_command(LCD_SETDDRAMADDR | (col + offsets[row]));
////}
////
////void lcd_puts(char *string) {
////  for (char *it = string; *it; it++) {
////    lcd_write(*it);
////  }
////}
////
////void lcd_printf(char *format, ...) {
////  va_list args;
////
////  va_start(args, format);
////  vsnprintf(lcd_buffer, LCD_COL_COUNT + 1, format, args);
////  va_end(args);
////
////  lcd_puts(lcd_buffer);
////}
