#include "main.h"
#include "LCD.h"
#include "Screens.h"
#include "HAL_BRD.h"

STATIC u8_t                         LCD_display_settings;
STATIC LCD_state_et                 LCD_state_s;


void LCD_init( void )
{
    LCD_display_settings = 0u;
    LCD_state_s = LCD_POWERING_UP;
}



void LCD_set_RS_pin( low_high_et state )
{
    ( state == HIGH ) ? HAL_BRD_LCD_set_RS_pin_high() : HAL_BRD_LCD_set_RS_pin_low();
}


void LCD_set_RW_pin( low_high_et state )
{
}


void LCD_set_EN_pin( low_high_et state )
{
    ( state == HIGH ) ? HAL_BRD_LCD_set_enable_pin_high() : HAL_BRD_LCD_set_enable_pin_low();
}


void LCD_blip_EN_pin_high_low( void )
{
	HAL_BRD_LCD_pulse_enable_pin_state();
}


void LCD_set_D0_pin( low_high_et state )
{
}


void LCD_set_D1_pin( low_high_et state )
{
}


void LCD_set_D2_pin( low_high_et state )
{
}


void LCD_set_D3_pin( low_high_et state )
{
}


void LCD_set_D4_pin( low_high_et state )
{
}


void LCD_set_D5_pin( low_high_et state )
{
}


void LCD_set_D6_pin( low_high_et state )
{
}


void LCD_set_D7_pin( low_high_et state )
{
}



pass_fail_et LCD_read_busy_flag( void )
{
    //low_high_et level = LOW;
    pass_fail_et status = FAIL;

    // delay_us( 1 );

    // /* First set RS LOW */
    // LCD_set_RS_pin( LOW );

    // /* Then set RW HIGH */
    // LCD_set_RW_pin( HIGH );

    // LCD_blip_EN_pin_high_low();

    // /* Now configure D7 as an input and then read it */
    // level == HAL_BRD_read_D7_pin();

    // if( level == LOW )
    // {
    //     status = PASS;
    // }

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
    // LCD_set_D0_pin( (low_high_et)( ( data&0x01 ) >> BIT0 ) );

    // /* D1 - D6  */
    // LCD_set_D1_pin( (low_high_et)( ( data&0x02 ) >> BIT1 ) );
    // LCD_set_D2_pin( (low_high_et)( ( data&0x04 ) >> BIT2 ) );
    // LCD_set_D3_pin( (low_high_et)( ( data&0x08 ) >> BIT3 ) );
    // LCD_set_D4_pin( (low_high_et)( ( data&0x10 ) >> BIT4 ) );
    // LCD_set_D5_pin( (low_high_et)( ( data&0x20 ) >> BIT5 ) );
    // LCD_set_D6_pin( (low_high_et)( ( data&0x40 ) >> BIT6 ) );

    // /* D7 */
    // LCD_set_D7_pin( (low_high_et)( ( data&0x80 ) >> BIT7 ) );

/* This is the most optimised way of shifting the data out on the shift register */
    ( data & 0x80 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x40 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x20 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x10 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x08 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x04 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x02 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();

	( data & 0x01 ) ? HAL_BRD_74HC164_set_data_pin_high() : HAL_BRD_74HC164_set_data_pin_low();
	HAL_BRD_74HC164_pulse_clk_pin_state();
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

    (void*)LCD_ready;

    if( type == LCD_COMMAND )
    {
        LCD_set_RS_pin( LOW );
    }
    else
    {
        LCD_set_RS_pin( HIGH );
    }

    //LCD_set_RW_pin( LOW );

    LCD_set_data_on_pins( byte );

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
        delay_us(40);
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

	if( size > LCD_COL_COUNT )
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




void LCD_tick( void )
{
    switch( LCD_state_s )
    {
        case LCD_POWERING_UP:
        {
            LCD_state_s = LCD_INITIALISING_P1;
        }
        break;

        case LCD_INITIALISING_P1:
        {
            LCD_send_command( LCD_FUNCTIONSET | LCD_8BITMODE );
            LCD_state_s = LCD_INITIALISING_P2;
        }
        break;

        case LCD_INITIALISING_P2:
        {
            LCD_send_command( LCD_FUNCTIONSET | LCD_8BITMODE );
            delay_us(120);
            LCD_send_command( LCD_FUNCTIONSET | LCD_8BITMODE );

            LCD_send_command( LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS );
			LCD_send_command( LCD_ENTRYMODESET | LCD_ENTRYLEFT );
			LCD_send_command( LCD_DISPLAYCONTROL | LCD_DISPLAYON );

			SCREENS_init();

			LCD_state_s = LCD_READY;
        }
        break;

        case LCD_READY:
        {
            /* We can now start displaying stuff :)  */
            SCREENS_handle_screen();
        }
        break;
    }
}






