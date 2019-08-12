#include "STDC.h"
#include "Screens.h"
#include "LCD.h"
#include "NVM.h"
#include "VERSIONS.H"


const u8_t MESSAGE_PROJECT[]     = ("  AIRCON  2019  ");
const u8_t MESSAGE_CREATOR[]     = ("  MARK STEWART  ");
const u8_t SETTINGS[]            = ("    SETTINGS    ");
const u8_t STATUS_ACTIVE[]       = ("     ACTIVE     ");
const u8_t BLANK[]               = ("                ");
const u8_t EXPERT_MODE[]         = ("  EXPERT MODE   ");


STATIC SCREENS_async_display_request_et SCREENS_async_display_request_s;
STATIC false_true_et                    SCREENS_button_press_s;
STATIC u16_t                            SCREENS_screen_timer_s;
STATIC u16_t                            SCREENS_loop_ctr_s;
STATIC SCREENS_screen_et                SCREEN_screen_s;

void SCREENS_init()
{
	SCREENS_screen_timer_s = 0u;
	SCREENS_loop_ctr_s = 0u;
    SCREENS_async_display_request_s = NO_REQUEST;
    SCREENS_button_press_s = FALSE;
    SCREEN_screen_s = WELCOME_MESSAGE_SCREEN;
}

void SCREENS_set_async_requests( SCREENS_async_display_request_et request )
{
   SCREENS_async_display_request_s = request;
}



void SCREENS_reset_screen_timer( void )
{
	SCREENS_screen_timer_s = 0u;
	SCREENS_loop_ctr_s = 0u;
}


void SCREENS_handle_screen( void )
{
    /* Create the screen */
    SCREENS_create_screen( SCREEN_screen_s );

    if( SCREENS_async_display_request_s != NO_REQUEST )
    {
        switch( SCREENS_async_display_request_s )
        {
            case INFO_REQUEST:
                SCREEN_screen_s = INFO_SCREEN_1;
                break;

            case EXPERT_MODE_REQUEST:
                SCREEN_screen_s = EXPERT_MODE_SCREEN;
                break;

            default:
                break;
        }
        SCREENS_async_display_request_s = NO_REQUEST;
        SCREENS_reset_screen_timer();
    }

    switch( SCREEN_screen_s )
    {
        case WELCOME_MESSAGE_SCREEN:
           SCREEN_screen_s = SCREENS_handle_welcome_screen();
        break;

        case MAIN_MENU_SCREEN:
            SCREEN_screen_s = SCREENS_handle_main_menu_screen( SCREENS_button_press_s );
            break;

        case INFO_SCREEN_1:
            SCREEN_screen_s = SCREENS_handle_info_screen_1( SCREENS_button_press_s );
            break;

        case INFO_SCREEN_2:
            SCREEN_screen_s = SCREENS_handle_info_screen_2( SCREENS_button_press_s );
            break;

        case INFO_SCREEN_3:
            SCREEN_screen_s = SCREENS_handle_info_screen_3( SCREENS_button_press_s );
            break;

        case EXPERT_MODE_SCREEN:
            SCREEN_screen_s = SCREENS_handle_expert_mode_screen();
            break;

        default:
            break;
    }
    SCREENS_button_press_s = FALSE;
}




void SCREENS_create_screen( SCREENS_screen_et screen )
{
    switch( screen )
    {
        case WELCOME_MESSAGE_SCREEN:
            SCREENS_create_welcome_screen();
            break;


        case MAIN_MENU_SCREEN:
            SCREENS_create_main_menu_screen();
            break;


        case INFO_SCREEN_1:
            SCREENS_create_info_screen_1();
            break;

        case INFO_SCREEN_2:
            SCREENS_create_info_screen_2();
            break;


        case EXPERT_MODE_SCREEN:
            SCREENS_create_expert_mode_screen();
            break;

        default:
        break;
    }
}



/*!! These should be moved out to a different file */
u8_t SCREENS_handle_welcome_screen( void )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = WELCOME_MESSAGE_SCREEN;

    /* The screen timer is counting up */
    if( SCREENS_screen_timer_s >= DISPLAY_WELCOME_SCREEN_TIMEOUT )
    {
        SCREENS_reset_screen_timer();
        new_screen = MAIN_MENU_SCREEN;
    }
    else
    {
        SCREENS_screen_timer_s++;
    }

   return( (SCREENS_screen_et)new_screen );
}








u8_t SCREENS_handle_main_menu_screen( false_true_et button_press )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = MAIN_MENU_SCREEN;

    if( button_press == TRUE )
    {
         new_screen = INFO_SCREEN_1;
    }

    return( new_screen );
}





u8_t SCREENS_handle_info_screen_1( false_true_et button_press )
{
    SCREENS_screen_et new_screen = INFO_SCREEN_1;

    if( button_press == TRUE )
    {
         new_screen = INFO_SCREEN_2;
    }

    return( new_screen );
}




u8_t SCREENS_handle_info_screen_2( false_true_et button_press )
{
    SCREENS_screen_et new_screen = INFO_SCREEN_2;

    if( button_press == TRUE )
    {
         new_screen = INFO_SCREEN_3;
    }

    return( new_screen );
}


u8_t SCREENS_handle_info_screen_3( false_true_et button_press )
{
    SCREENS_screen_et new_screen = INFO_SCREEN_3;

    if( button_press == TRUE )
    {
         new_screen = MAIN_MENU_SCREEN;
    }

    return( new_screen );
}





u8_t SCREENS_handle_expert_mode_screen( void )
{
    SCREENS_screen_et new_screen = EXPERT_MODE_SCREEN;

    /* The screen timer is counting up */
    if( SCREENS_screen_timer_s >= DISPLAY_WELCOME_SCREEN_TIMEOUT )
    {
        SCREENS_reset_screen_timer();
        new_screen = MAIN_MENU_SCREEN;
    }
    else
    {
        SCREENS_screen_timer_s++;
    }

    return( new_screen );
}









void SCREENS_create_welcome_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */

	/* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ];
    u8_t i = 0x20;

    STDC_memcpy( message, &i, sizeof( message ) );

    switch( SCREENS_loop_ctr_s )
    {
		case 0 ... LCD_COL_COUNT:
		{
			for( i = 0u; i < SCREENS_loop_ctr_s; i++ )
			{
				message[i] = '*';
			}

			LCD_set_cursor_position(0,0);
			LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

			LCD_set_cursor_position(1,0);
			LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
		}
		break;

		case ( LCD_COL_COUNT + 1u ) ... ( LCD_COL_COUNT + 10 ): break;

		default:
			LCD_set_cursor_position(0,0);
			LCD_write_message( (u8_t*)MESSAGE_PROJECT, LCD_COL_COUNT );

			LCD_set_cursor_position(1,0);
			LCD_write_message( (u8_t*)MESSAGE_CREATOR, LCD_COL_COUNT );
			break;
    }

    SCREENS_loop_ctr_s ++;
}





void SCREENS_create_main_menu_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ];

    u16_t node = 0x0055;
    u8_t temp_1 = 40;
    u8_t temp_2 = 5;

    STDC_memset( message, 0x20, sizeof(message) );
    sprintf( message, "N:0x%04d T:%02d.%d",node, temp_1, temp_2 );

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );
    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}






void SCREENS_create_info_screen_1( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ];

    STDC_memset( message, 0x20, sizeof(message) );
    sprintf( message, "  Aircon  Unit  ");

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );
    sprintf( message, "      2019      ");

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
}



void SCREENS_create_info_screen_2( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    u8_t sw_version_string[SW_VERSION_NUM_SIZE];
    u8_t hw_version_string[HW_VERSION_NUM_SIZE];

    HAL_BRD_get_SW_version_num( sw_version_string );
    HAL_BRD_get_HW_version_num( hw_version_string );

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    
    /* Now copy the const string into the new temp buffer */
    sprintf( message, "SW Vers:%02d.%02d.%02d",  
    sw_version_string[0], sw_version_string[1], sw_version_string[2] );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    LCD_set_cursor_position(1,0);

    /* Now copy the const string into the new temp buffer */
    sprintf( message, "HW Vers:%02d.%02d.%02d",  
    hw_version_string[0], hw_version_string[1], hw_version_string[2] );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}






void SCREENS_create_expert_mode_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
}
