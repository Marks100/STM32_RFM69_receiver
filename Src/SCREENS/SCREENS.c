#include "STDC.h"
#include "Screens.h"
#include "LCD.h"
#include "NVM.h"
#include "VERSIONS.H"


const u8_t MESSAGE_SPOT_WELDER[] = ("SPOT WELDER 2018");
const u8_t MESSAGE_CREATOR[]     = ("  MARK STEWART  ");
const u8_t MESSAGE_SAFETY_0[]    = ("*   EXTREMELY  *");
const u8_t MESSAGE_SAFETY_1[] 	 = ("*   DANGEROUS  *");
const u8_t MESSAGE_SAFETY_2[]    = ("* PLEASE TREAT *");
const u8_t MESSAGE_SAFETY_3[]    = ("* WITH CAUTION *");
const u8_t FIRING[]              = ("!!.. FIRING ..!!");
const u8_t ARMED[]               = ("!!    ARMED   !!");
const u8_t DISARMED[]            = ("    DISARMED    ");
const u8_t SETTINGS[]            = ("    SETTINGS    ");
const u8_t WELD_TIME[]           = ("WELD TIME:0000ms");
const u8_t STATUS_ACTIVE[]       = ("     ACTIVE     ");
const u8_t BLANK[]               = ("                ");
const u8_t WIRE_HOT[]            = ("  WIRING HOT!!  ");
const u8_t COOLDOWN[]            = ("COOLDOWN NEEDED ");
const u8_t TOTAL_WELDS[]         = ("TOT WELDS: 00000");
const u8_t EXPERT_MODE[]         = ("  EXPERT MODE   ");
const u8_t NO_COOLDOWN[]         = ("NO COOLDOWN TIME");
const u8_t SW_VERSION[]          = ("SW VERS:00.00.00");
const u8_t HW_VERSION[]          = ("HW VERS:00.00   ");
const u8_t TEMPERATURE[]         = ("IN Temp: 000 deg");
const u8_t COOL[]                = ("     COOL       ");
const u8_t WARM[]                = ("     WARM       ");
const u8_t HOT[]                 = ("   !! HOT  !!   ");


STATIC SCREENS_async_display_request_et SCREENS_async_display_request_s;
STATIC false_true_et                    SCREENS_button_press_s;
STATIC u16_t                            SCREENS_screen_timer_s;
STATIC SCREENS_screen_et                SCREEN_screen_s;

void SCREENS_init()
{
	SCREENS_screen_timer_s = 0u;
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
}


void SCREENS_handle_screen( void )
{
    /* Create the screen */
    SCREENS_create_screen( SCREEN_screen_s );

    if( SCREENS_async_display_request_s != NO_REQUEST )
    {
        switch( SCREENS_async_display_request_s )
        {
            case WELDER_FIRING_REQUEST:
                SCREEN_screen_s = FIRING_SCREEN;
            break;

            case DUTY_CYCLE_WARNING_REQUEST:
                SCREEN_screen_s = DUTY_CYCLE_WARNING_SCREEN;
                break;

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

        case SAFETY_INFO_P1_SCREEN:
            SCREEN_screen_s = SCREENS_handle_safety_message_1_screen();
            break;

        case SAFETY_INFO_P2_SCREEN:
            SCREEN_screen_s = SCREENS_handle_safety_message_2_screen();
            break;

        case MAIN_MENU_SCREEN:
            SCREEN_screen_s = SCREENS_handle_main_menu_screen( SCREENS_button_press_s );
            break;

        case FIRING_SCREEN:
            SCREEN_screen_s = SCREENS_handle_firing_screen();
            break;

        case DUTY_CYCLE_WARNING_SCREEN:
            SCREEN_screen_s = SCREENS_handle_duty_cycle_warning_screen();
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

        case SAFETY_INFO_P1_SCREEN:
            SCREENS_create_safety_message_1_screen();
            break;

        case SAFETY_INFO_P2_SCREEN:
            SCREENS_create_safety_message_2_screen();
            break;

        case MAIN_MENU_SCREEN:
            SCREENS_create_main_menu_screen();
            break;

        case FIRING_SCREEN:
            SCREENS_create_firing_screen();
            break;

        case DUTY_CYCLE_WARNING_SCREEN:
            SCREENS_create_duty_cycle_warning_screen();
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
        new_screen = SAFETY_INFO_P1_SCREEN;
    }
    else
    {
        SCREENS_screen_timer_s++;
    }

   return( (SCREENS_screen_et)new_screen );
}



u8_t SCREENS_handle_safety_message_1_screen( void )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = SAFETY_INFO_P1_SCREEN;

    /* The screen timer is counting up */
    if( SCREENS_screen_timer_s >= DISPLAY_WELCOME_SCREEN_TIMEOUT )
    {
        SCREENS_reset_screen_timer();
        new_screen = SAFETY_INFO_P2_SCREEN;
    }
    else
    {
        SCREENS_screen_timer_s++;
    }

   return( new_screen );
}




u8_t SCREENS_handle_safety_message_2_screen( void )
{
    /* keep track of the last screen that we were on */
    SCREENS_screen_et new_screen = SAFETY_INFO_P2_SCREEN;


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



u8_t SCREENS_handle_firing_screen( void )
{
    SCREENS_screen_et new_screen = FIRING_SCREEN;

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



u8_t SCREENS_handle_duty_cycle_warning_screen( void )
{
    SCREENS_screen_et new_screen = DUTY_CYCLE_WARNING_SCREEN;

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
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)MESSAGE_SPOT_WELDER, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_CREATOR, LCD_COL_COUNT );
}



void SCREENS_create_safety_message_1_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_0, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_1, LCD_COL_COUNT );
}



void SCREENS_create_safety_message_2_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_2, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_3, LCD_COL_COUNT );
}



void SCREENS_create_main_menu_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)SW_VERSION, LCD_COL_COUNT );


    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    /* Now copy the const string into the new temp buffer */
        STDC_memcpy( (u8_t*)message, (u8_t*)HW_VERSION, LCD_COL_COUNT );


    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)STATUS_ACTIVE, LCD_COL_COUNT );
}


void SCREENS_create_firing_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)FIRING, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
}





void SCREENS_create_duty_cycle_warning_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)WIRE_HOT, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)COOLDOWN, LCD_COL_COUNT );
}



void SCREENS_create_info_screen_1( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    u32_t num_welds = 0u;


    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
}



void SCREENS_create_info_screen_2( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    u32_t num_welds = 0;
    u8_t num;

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)SW_VERSION, LCD_COL_COUNT );

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)HW_VERSION, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void SCREENS_create_info_screen_3( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    s8_t num;
    u8_t temp;

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)TEMPERATURE, LCD_COL_COUNT );

    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);

    if( temp < 50u )
    {
        /* Now copy the const string into the new temp buffer */
        STDC_memcpy( (u8_t*)message, (u8_t*)COOL, LCD_COL_COUNT );
    }
    else if( ( temp > 50u ) && ( temp < 80 ) )
    {
        /* Now copy the const string into the new temp buffer */
        STDC_memcpy( (u8_t*)message, (u8_t*)WARM, LCD_COL_COUNT );
    }
    else
    {
        /* Now copy the const string into the new temp buffer */
        STDC_memcpy( (u8_t*)message, (u8_t*)HOT, LCD_COL_COUNT );
    }
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}







void SCREENS_create_expert_mode_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)EXPERT_MODE, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)NO_COOLDOWN, LCD_COL_COUNT );
}

