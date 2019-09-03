#include <stdio.h>
#include <string.h>

#include "STDC.h"
#include "Screens.h"
#include "LCD.h"
#include "HAL_BRD.h"
#include "AIRCON.h"
#include "NVM.h"
#include "VERSIONS.H"


const u8_t MESSAGE_PROJECT[]     = ("  AIRCON  2019  ");
const u8_t MESSAGE_CREATOR[]     = ("  MARK STEWART  ");
const u8_t SETTINGS[]            = ("    SETTINGS    ");
const u8_t EXPERT_MODE[]         = ("   EXPERT MODE  ");
const u8_t BLANK[]               = ("                ");

SCREENS_main_memu_items_st SCREENS_main_memu_items_s = 
{
	0,
	{
        MONITOR_SCREEN,
		SET_TEMP_SEL_SCREEN,
        INFO_SCREEN_1,
		RESET_SCREEN,
		MODE_SELECT_SCREEN,
		STATE_SELECT_SCREEN,
		MAIN_MENU_SCREEN,
		MAIN_MENU_SCREEN,
		MAIN_MENU_SCREEN
	},
	{
        "1. MONITOR      ",
		"2. SET TEMP     ",
		"3. INFO         ",
		"4. RESET        ",
		"5. MODE SELECT  ",
		"6. STATE SELECT ",
		"7. Option 3     ",
		"8. Option 4     ",
		"9. Option 5     ",
	},
};

SCREENS_info_memu_items_st SCREENS_info_memu_items_s =
{

	0,
	{
		INFO_SCREEN_1,
		INFO_SCREEN_2,
		INFO_SCREEN_3,
		INFO_SCREEN_4
	},
};

SCREENS_set_temp_memu_items_st SCREENS_set_temp_memu_items_s =
{
	0,
	{
		SET_AUTO_TEMP_SCREEN, //SET_AUTO_TEMP_SCREEN,
		SET_HEAT_TEMP_SCREEN, //SET_HEAT_TEMP_SCREEN,
		SET_COOL_TEMP_SCREEN, //SET_COOL_TEMP_SCREEN,
		MAIN_MENU_SCREEN
	},
	{
		"1. AUTO TEMP    ",
		"2. HEAT TEMP    ",
		"3. COOL TEMP    ",
		"BACK            "
	},
};



STATIC SCREENS_async_display_request_et SCREENS_async_display_request_s;
STATIC ROTARY_scroll_type_et            SCREENS_rotary_press_s;
STATIC u16_t                            SCREENS_screen_timer_s;
STATIC u32_t                            SCREENS_loop_ctr_s;
STATIC SCREENS_screen_et                SCREEN_screen_s;
STATIC u16_t                            SCREEN_inactivity_timer_s;


void SCREENS_init()
{
	SCREENS_screen_timer_s = 0u;
	SCREENS_loop_ctr_s = 0u;
    SCREENS_async_display_request_s = NO_REQUEST;
    SCREENS_rotary_press_s = ROTARY_NO_CHANGE;
    SCREEN_screen_s = WELCOME_MESSAGE_SCREEN;
    SCREEN_inactivity_timer_s = 0u;

    SCREENS_main_memu_items_s.item_no = 0u;
}


void SCREENS_handle_rotary_input( ROTARY_scroll_type_et type )
{
    if( SCREENS_rotary_press_s == ROTARY_NO_CHANGE )
    {
        SCREENS_rotary_press_s = type;

        /* Button pressed so reset inactivity timer */
        SCREENS_reset_inactivity_timer();
    }
}


void SCREENS_handle_inactivity_timer( void )
{
    if( SCREEN_inactivity_timer_s < DISPLAY_INACTIVITY_GENERAL_TIME )
    {
        SCREEN_inactivity_timer_s ++;
    } 
    else
    {
        SCREENS_reset_inactivity_timer();

        /* Set main screen */
        SCREEN_screen_s = MONITOR_SCREEN;

        SCREENS_main_memu_items_s.item_no = 0u;
        SCREENS_info_memu_items_s.item_no = 0u;
        SCREENS_set_temp_memu_items_s.item_no = 0u;
    }
}


void SCREENS_reset_inactivity_timer( void )
{
    SCREEN_inactivity_timer_s = 0u;
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
            SCREEN_screen_s = SCREENS_handle_main_menu_screen( SCREENS_rotary_press_s );
            break;

        case INFO_SCREEN_1:
        case INFO_SCREEN_2:
        case INFO_SCREEN_3:
        case INFO_SCREEN_4:
            SCREEN_screen_s = SCREENS_handle_info_screens( SCREENS_rotary_press_s );
            break;

        case TEST_SCREEN:
            SCREEN_screen_s = SCREENS_handle_test_screen( SCREENS_rotary_press_s );
            break;

        case EXPERT_MODE_SCREEN:
            SCREEN_screen_s = SCREENS_handle_expert_mode_screen();
            break;

        case SET_TEMP_SEL_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_temp_sel_screen( SCREENS_rotary_press_s );
        	break;

        case SET_AUTO_TEMP_SCREEN:
            SCREEN_screen_s = SCREENS_handle_auto_temp_screen( SCREENS_rotary_press_s );
            break;

        case SET_COOL_TEMP_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_cool_temp_screen( SCREENS_rotary_press_s );
        	break;

        case SET_HEAT_TEMP_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_heat_temp_screen( SCREENS_rotary_press_s );
        	break;

        case MONITOR_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_monitor_screen( SCREENS_rotary_press_s );
        	break;

        case MODE_SELECT_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_mode_selector_screen( SCREENS_rotary_press_s );
            break;

        case STATE_SELECT_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_state_selector_screen( SCREENS_rotary_press_s );
            break;

        case RESET_SCREEN:
        	SCREEN_screen_s = SCREENS_handle_reset_screen( SCREENS_rotary_press_s );
        	break;

        default:
            break;
    }
    SCREENS_rotary_press_s = ROTARY_NO_CHANGE;
    SCREENS_handle_inactivity_timer();
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

        case INFO_SCREEN_3:
            SCREENS_create_info_screen_3();
            break;

        case INFO_SCREEN_4:
        	SCREENS_create_info_screen_4();
        	break;

        case TEST_SCREEN:
        	SCREENS_create_test_screen();
        	break;

        case EXPERT_MODE_SCREEN:
            SCREENS_create_expert_mode_screen();
            break;

        case SET_TEMP_SEL_SCREEN:
        	SCREENS_create_temp_sel_screen();
        	break;

        case SET_AUTO_TEMP_SCREEN:
            SCREENS_create_auto_temp_screen();
            break;

        case SET_COOL_TEMP_SCREEN:
        	SCREENS_create_cool_temp_screen();
        	break;

        case SET_HEAT_TEMP_SCREEN:
        	SCREENS_create_heat_temp_screen();
        	break;

        case MONITOR_SCREEN:
        	SCREENS_create_monitor_screen();
        	break;

        case MODE_SELECT_SCREEN:
			SCREENS_create_mode_selector_screen();
			break;

        case STATE_SELECT_SCREEN:
            SCREENS_create_state_selector_screen();
            break;

        case RESET_SCREEN:
            SCREENS_create_reset_screen();
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
        new_screen = MONITOR_SCREEN;
    }
    else
    {
        SCREENS_screen_timer_s++;
    }

   return( (SCREENS_screen_et)new_screen );
}



u8_t SCREENS_handle_main_menu_screen( ROTARY_scroll_type_et button_press )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = MAIN_MENU_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
         if( SCREENS_main_memu_items_s.item_no == 0 )
         {
        	 SCREENS_main_memu_items_s.item_no = SCREENS_MAIN_MEMU_ITEMS - 1;
         }
         else
         {
        	 SCREENS_main_memu_items_s.item_no -= 1;
         }
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	SCREENS_main_memu_items_s.item_no = ( ( SCREENS_main_memu_items_s.item_no + 1 ) % SCREENS_MAIN_MEMU_ITEMS);
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = SCREENS_main_memu_items_s.screens[SCREENS_main_memu_items_s.item_no];
    }


    /* ive added this here for now, everytime we get back to the main menu we want to reset the
     * selection items for some of the other screens
     */
    SCREENS_set_temp_memu_items_s.item_no = 0u;
    SCREENS_info_memu_items_s.item_no     = 0u;

    return( new_screen );
}




u8_t SCREENS_handle_set_temp_screen( ROTARY_scroll_type_et button_press )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = SET_TEMP_SEL_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
         if( SCREENS_set_temp_memu_items_s.item_no == 0 )
         {
        	 SCREENS_set_temp_memu_items_s.item_no = SCREENS_SET_TEMP_MEMU_ITEMS - 1;
         }
         else
         {
        	 SCREENS_set_temp_memu_items_s.item_no -= 1;
         }
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	SCREENS_set_temp_memu_items_s.item_no = ( ( SCREENS_set_temp_memu_items_s.item_no + 1 ) % SCREENS_SET_TEMP_MEMU_ITEMS);
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = SCREENS_set_temp_memu_items_s.screens[SCREENS_set_temp_memu_items_s.item_no];
    }

    return( new_screen );
}



u8_t SCREENS_handle_monitor_screen( ROTARY_scroll_type_et button_press )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = MONITOR_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
	{
	}
	else if( button_press == ROTARY_RIGHT_SCROLL )
	{
	}
	else if( button_press == ROTARY_SHORT_PPRESS )
	{
		new_screen = MAIN_MENU_SCREEN;
	}

    return( new_screen );
}





u8_t SCREENS_handle_info_screens( ROTARY_scroll_type_et button_press )
{
    /* keep track of the last screen that we were on */
	SCREENS_screen_et new_screen = INFO_SCREEN_1;

	if( button_press == ROTARY_LEFT_SCROLL )
	{
		 if( SCREENS_info_memu_items_s.item_no == 0 )
		 {
			 SCREENS_info_memu_items_s.item_no = SCREENS_INFO_MEMU_ITEMS - 1;
		 }
		 else
		 {
			 SCREENS_info_memu_items_s.item_no -= 1;
		 }
	}
	else if( button_press == ROTARY_RIGHT_SCROLL )
	{
		SCREENS_info_memu_items_s.item_no = ( ( SCREENS_info_memu_items_s.item_no + 1 ) % SCREENS_INFO_MEMU_ITEMS);
	}

	if( button_press == ROTARY_SHORT_PPRESS )
	{
		new_screen = MAIN_MENU_SCREEN;
	}
	else
	{
		new_screen = SCREENS_info_memu_items_s.screens[SCREENS_info_memu_items_s.item_no];
	}

    return( new_screen );
}




u8_t SCREENS_handle_info_screen_2( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = INFO_SCREEN_2;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
         new_screen = INFO_SCREEN_3;
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	new_screen = INFO_SCREEN_1;
    }

    return( new_screen );
}


u8_t SCREENS_handle_info_screen_3( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = INFO_SCREEN_3;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
         new_screen = TEST_SCREEN;
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	new_screen = INFO_SCREEN_2;
    }

    return( new_screen );
}


SCREENS_screen_et SCREENS_handle_test_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = TEST_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
         new_screen = SET_AUTO_TEMP_SCREEN;
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	new_screen = INFO_SCREEN_3;
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


u8_t SCREENS_handle_auto_temp_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = SET_AUTO_TEMP_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
    	AIRCON_adjust_temperature_setting(AIRCON_AUTO_MODE, SETTING_ADJUST_DOWN );
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	AIRCON_adjust_temperature_setting(AIRCON_AUTO_MODE, SETTING_ADJUST_UP );
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = SET_TEMP_SEL_SCREEN;
    }

    return( new_screen );
}


u8_t SCREENS_handle_cool_temp_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = SET_COOL_TEMP_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
    	AIRCON_adjust_temperature_setting(AIRCON_COOL_MODE, SETTING_ADJUST_DOWN );
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	AIRCON_adjust_temperature_setting(AIRCON_COOL_MODE, SETTING_ADJUST_UP );
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = SET_TEMP_SEL_SCREEN;
    }

    return( new_screen );
}


u8_t SCREENS_handle_heat_temp_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = SET_HEAT_TEMP_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
     	AIRCON_adjust_temperature_setting(AIRCON_HEAT_MODE, SETTING_ADJUST_DOWN );
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
     	AIRCON_adjust_temperature_setting(AIRCON_HEAT_MODE, SETTING_ADJUST_UP );
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = SET_TEMP_SEL_SCREEN;
    }

    return( new_screen );
}



u8_t SCREENS_handle_temp_sel_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = SET_TEMP_SEL_SCREEN;

   if( button_press == ROTARY_LEFT_SCROLL )
	{
        if( SCREENS_set_temp_memu_items_s.item_no == 0 )
        {
        	SCREENS_set_temp_memu_items_s.item_no = SCREENS_SET_TEMP_MEMU_ITEMS - 1;
        }
        else
        {
        	SCREENS_set_temp_memu_items_s.item_no -= 1;
        }
	}
	else if( button_press == ROTARY_RIGHT_SCROLL )
	{
		SCREENS_set_temp_memu_items_s.item_no = ( ( SCREENS_set_temp_memu_items_s.item_no + 1 ) % SCREENS_SET_TEMP_MEMU_ITEMS);
	}

	if( button_press == ROTARY_SHORT_PPRESS )
	{
		new_screen = SCREENS_set_temp_memu_items_s.screens[SCREENS_set_temp_memu_items_s.item_no];
	}

	return( new_screen );
}



u8_t SCREENS_handle_mode_selector_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = MODE_SELECT_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
    	AIRCON_toggle_mode();
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	AIRCON_toggle_mode();
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = MAIN_MENU_SCREEN;
    }

    return( new_screen );
}


u8_t SCREENS_handle_state_selector_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = STATE_SELECT_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
    	( AIRCON_get_state() == ENABLE_ ) ? AIRCON_set_state( DISABLE_ ) : AIRCON_set_state( ENABLE_ );
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    	( AIRCON_get_state() == ENABLE_ ) ? AIRCON_set_state( DISABLE_ ) : AIRCON_set_state( ENABLE_ );
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    	new_screen = MAIN_MENU_SCREEN;
    }

    return( new_screen );
}



u8_t SCREENS_handle_reset_screen( ROTARY_scroll_type_et button_press )
{
    SCREENS_screen_et new_screen = RESET_SCREEN;

    if( button_press == ROTARY_LEFT_SCROLL )
    {
    }
    else if( button_press == ROTARY_RIGHT_SCROLL )
    {
    }
    else if( button_press == ROTARY_SHORT_PPRESS )
    {
    }

    if( SCREENS_screen_timer_s >= DISPLAY_RESET_SCREEN_TIMEOUT )
    {
        /* reset the device */
        SCREENS_reset_screen_timer();
        new_screen = MAIN_MENU_SCREEN;

        NVM_request_flush();
	    NVIC_SystemReset();
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

		case ( LCD_COL_COUNT + 1u ) ... ( LCD_COL_COUNT + 5 ): break;

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
	u8_t message[ LCD_COL_COUNT + 1u ];


	sprintf( message, ">%s", SCREENS_main_memu_items_s.list[SCREENS_main_memu_items_s.item_no] );
	LCD_set_cursor_position(0,0);
	LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

	LCD_set_cursor_position(1,0);
	if( SCREENS_main_memu_items_s.item_no == SCREENS_MAIN_MEMU_ITEMS - 1 )
	{
		LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
	}
	else
	{
		sprintf( message, "%s", SCREENS_main_memu_items_s.list[SCREENS_main_memu_items_s.item_no+1] );
		LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
	}
}



void SCREENS_create_set_temp_screen( void )
{
    /* Create a non const array to hold the LCD string */
	u8_t message[ LCD_COL_COUNT + 1u ];

	sprintf( message, ">%s", SCREENS_set_temp_memu_items_s.list[SCREENS_set_temp_memu_items_s.item_no] );
	LCD_set_cursor_position(0,0);
	LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

	LCD_set_cursor_position(1,0);
	if( SCREENS_set_temp_memu_items_s.item_no == SCREENS_SET_TEMP_MEMU_ITEMS - 1 )
	{
		LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
	}
	else
	{
		sprintf( message, "%s", SCREENS_set_temp_memu_items_s.list[SCREENS_set_temp_memu_items_s.item_no+1] );
		LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
	}
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
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void SCREENS_create_info_screen_2( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
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
    sprintf( message, "HW Vers:%02d.%02d   ",
    hw_version_string[0], hw_version_string[1] );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}


void SCREENS_create_info_screen_3( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];

    /* Display the message and keep it on the screen until the timer expires */
    sprintf( message, "  Developed By  " );
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );
    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_CREATOR, LCD_COL_COUNT );
}

void SCREENS_create_info_screen_4( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];

    /* Display the message and keep it on the screen until the timer expires */
    sprintf( message, "   Build Date   " );
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );
    sprintf( message, "   22/08/2019   " );
    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}


void SCREENS_create_test_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, "ctr:%010d  ", SCREENS_loop_ctr_s );

    /* Now copy the const string into the new temp buffer */
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    SCREENS_loop_ctr_s ++;
}


void SCREENS_create_auto_temp_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
    u16_t target_temp_c = 0u;

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, "Auto target temp" );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    target_temp_c = ( AIRCON_get_temperature_setting(AIRCON_AUTO_MODE) * 10 );
    LCD_set_cursor_position(1,0);
    sprintf( message, "  %d.%dc         ", ( target_temp_c / 10 ), ( target_temp_c % 10 ) );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}


void SCREENS_create_temp_sel_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];

	sprintf( message, ">%s", SCREENS_set_temp_memu_items_s.list[SCREENS_set_temp_memu_items_s.item_no] );
	LCD_set_cursor_position(0,0);
	LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

	LCD_set_cursor_position(1,0);
	if( SCREENS_set_temp_memu_items_s.item_no == SCREENS_SET_TEMP_MEMU_ITEMS - 1 )
	{
		LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
	}
	else
	{
		sprintf( message, "%s", SCREENS_set_temp_memu_items_s.list[SCREENS_set_temp_memu_items_s.item_no+1] );
		LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
	}
}



void SCREENS_create_heat_temp_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
    u16_t target_temp_c = 0u;

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, " Heat max temp  " );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    target_temp_c = ( AIRCON_get_temperature_setting(AIRCON_HEAT_MODE) * 10 );
    LCD_set_cursor_position(1,0);
    sprintf( message, "  %d.%dc         ", ( target_temp_c / 10 ), ( target_temp_c % 10 ) );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void SCREENS_create_cool_temp_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
    u16_t target_temp_c = 0u;

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, " Cool min temp  " );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    target_temp_c = ( AIRCON_get_temperature_setting(AIRCON_COOL_MODE) * 10 );
    LCD_set_cursor_position(1,0);
    sprintf( message, "  %d.%dc         ", ( target_temp_c / 10 ), ( target_temp_c % 10 ) );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void SCREENS_create_reset_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, "Resetting...    " );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
}


void SCREENS_create_mode_selector_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
    u8_t* strings[3] = { "HEAT", "COOL", "AUTO" };

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, "     Mode       " );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    sprintf( message, "   %s MODE    ", strings[ AIRCON_get_mode() ] );
    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void SCREENS_create_state_selector_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
    u8_t* strings[2] = { "OFF", "ON " };

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);

    sprintf( message, "     State      " );
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    STDC_memset( message, 0x20, sizeof(message) );

    sprintf( message, "      %s       ", strings[ AIRCON_get_state() ] );
    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void SCREENS_create_monitor_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT + 1u ];
	float temp_c;

	u8_t* enable_disable_strings[2] = { "OFF", "ON " };
	u8_t* mode_strings[3] = { "HEAT", "COOL", "AUTO" };

	temp_c =  AIRCON_get_oat();

	STDC_memset( message, 0x20, sizeof(message) );
	sprintf( message,     		 	      "%s ", enable_disable_strings[AIRCON_get_state()] );
	sprintf( &message[strlen( message )], "%s " , mode_strings[AIRCON_get_mode()] );

	if( temp_c >= TMPERATURE_NOT_AVAILABLE )
	{
	sprintf( &message[strlen( message )], "N/A     " );
	}
	else
	{
	sprintf( &message[strlen( message )], "%02d.%dc   ", (s16_t)( (temp_c * 10 ) / 10 ), ( (u16_t)( temp_c * 10 ) % 10 )  );
	}

	LCD_set_cursor_position(0,0);
	LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

	STDC_memset(message, 0x20, sizeof(message) );
	sprintf( message, "F:%s H:%s     ", enable_disable_strings[ AIRCON_get_cooler_state() ], \
									 enable_disable_strings[ AIRCON_get_heater_state() ] );

	LCD_set_cursor_position(1,0);
	LCD_write_message( (u8_t*)message, LCD_COL_COUNT  );
}






void SCREENS_create_expert_mode_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
}

