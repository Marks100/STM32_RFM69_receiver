#include "Screens.h"
#include "LCD.h"
#include "NVM.h"
#include "VERSIONS.H"


static u16_t LCD_screen_timer_s;



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




void Screens_init()
{
	LCD_screen_timer_s = 0u;
}



void LCD_reset_screen_timer( void )
{
	LCD_screen_timer_s = 0u;
}




/*!! These should be moved out to a different file */
u8_t LCD_handle_welcome_screen( void )
{
    /* keep track of the last screen that we were on */
    LCD_screen_et new_screen = LCD_WELCOME_MESSAGE_SCREEN;

    /* The screen timer is counting up */
    if( LCD_screen_timer_s >= LCD_DISPLAY_WELCOME_SCREEN_TIMEOUT )
    {
        LCD_reset_screen_timer();
        new_screen = LCD_SAFETY_INFO_P1_SCREEN;
    }
    else
    {
        LCD_screen_timer_s++;
    }

   return( (LCD_screen_et)new_screen );
}



u8_t LCD_handle_safety_message_1_screen( void )
{
    /* keep track of the last screen that we were on */
    LCD_screen_et new_screen = LCD_SAFETY_INFO_P1_SCREEN;

    /* The screen timer is counting up */
    if( LCD_screen_timer_s >= LCD_DISPLAY_WELCOME_SCREEN_TIMEOUT )
    {
        LCD_reset_screen_timer();
        new_screen = LCD_SAFETY_INFO_P2_SCREEN;
    }
    else
    {
        LCD_screen_timer_s++;
    }

   return( new_screen );
}




u8_t LCD_handle_safety_message_2_screen( void )
{
    /* keep track of the last screen that we were on */
    LCD_screen_et new_screen = LCD_SAFETY_INFO_P2_SCREEN;


    /* The screen timer is counting up */
    if( LCD_screen_timer_s >= LCD_DISPLAY_WELCOME_SCREEN_TIMEOUT )
    {
        LCD_reset_screen_timer();
        new_screen = LCD_MAIN_MENU_SCREEN;
    }
    else
    {
        LCD_screen_timer_s++;
    }

   return( new_screen );
}




u8_t LCD_handle_main_menu_screen( false_true_et button_press )
{
    /* keep track of the last screen that we were on */
    LCD_screen_et new_screen = LCD_MAIN_MENU_SCREEN;

     if( button_press == TRUE )
    {
         new_screen = LCD_INFO_SCREEN_1;
    }

    return( new_screen );
}



u8_t LCD_handle_firing_screen( void )
{
    LCD_screen_et new_screen = LCD_FIRING_SCREEN;

    /* The screen timer is counting up */
    if( LCD_screen_timer_s >= LCD_WELDER_FIRING_SCREEN_TIMEOUT_MS )
    {
        LCD_reset_screen_timer();
        new_screen = LCD_MAIN_MENU_SCREEN;
    }
    else
    {
        LCD_screen_timer_s++;
    }

    return( new_screen );
}



u8_t LCD_handle_duty_cycle_warning_screen( void )
{
    LCD_screen_et new_screen = LCD_DUTY_CYCLE_WARNING_SCREEN;

    /* The screen timer is counting up */
    if( LCD_screen_timer_s >= LCD_WELDER_HOT_WARNING_TIMEOUT_MS )
    {
        LCD_reset_screen_timer();
        new_screen = LCD_MAIN_MENU_SCREEN;
    }
    else
    {
        LCD_screen_timer_s++;
    }

    return( new_screen );
}




u8_t LCD_handle_info_screen_1( false_true_et button_press )
{
    LCD_screen_et new_screen = LCD_INFO_SCREEN_1;

    if( button_press == TRUE )
    {
         new_screen = LCD_INFO_SCREEN_2;
    }

    return( new_screen );
}




u8_t LCD_handle_info_screen_2( false_true_et button_press )
{
    LCD_screen_et new_screen = LCD_INFO_SCREEN_2;

    if( button_press == TRUE )
    {
         new_screen = LCD_INFO_SCREEN_3;
    }

    return( new_screen );
}


u8_t LCD_handle_info_screen_3( false_true_et button_press )
{
    LCD_screen_et new_screen = LCD_INFO_SCREEN_3;

    if( button_press == TRUE )
    {
         new_screen = LCD_MAIN_MENU_SCREEN;
    }

    return( new_screen );
}





u8_t LCD_handle_expert_mode_screen( void )
{
    LCD_screen_et new_screen = LCD_EXPERT_MODE_SCREEN;

    /* The screen timer is counting up */
    if( LCD_screen_timer_s >= LCD_WELDER_HOT_WARNING_TIMEOUT_MS )
    {
        LCD_reset_screen_timer();
        new_screen = LCD_MAIN_MENU_SCREEN;
    }
    else
    {
        LCD_screen_timer_s++;
    }

    return( new_screen );
}









void LCD_create_welcome_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)MESSAGE_SPOT_WELDER, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_CREATOR, LCD_COL_COUNT );
}



void LCD_create_safety_message_1_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_0, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_1, LCD_COL_COUNT );
}



void LCD_create_safety_message_2_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_2, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)MESSAGE_SAFETY_3, LCD_COL_COUNT );
}



void LCD_create_main_menu_screen( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    u16_t weld_time_ms = MODE_MGR_get_spot_weld_time();

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)WELD_TIME, LCD_COL_COUNT );

    /* Now modify the new buffer */
    message[10] = ( ( weld_time_ms / 1000 ) | 0x30 );
    weld_time_ms -= ( ( message[10] - 0x30 ) * 1000 );

    message[11] = ( ( weld_time_ms / 100 ) | 0x30 );
    weld_time_ms -= ( ( message[11] - 0x30 ) * 100 );

    message[12] = ( ( weld_time_ms / 10 ) | 0x30 );
    weld_time_ms -= ( ( message[12] - 0x30 ) * 10 );

    message[13] = ( weld_time_ms | 0x30 );

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)STATUS_ACTIVE, LCD_COL_COUNT );
}


void LCD_create_firing_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)FIRING, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
}





void LCD_create_duty_cycle_warning_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)WIRE_HOT, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)COOLDOWN, LCD_COL_COUNT );
}



void LCD_create_info_screen_1( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    u32_t num_welds = NVM_info_s.NVM_generic_data_blk_s.total_num_welds;

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)TOTAL_WELDS, LCD_COL_COUNT );

    /* Now modify the new buffer */
    message[11] = ( ( num_welds / 10000 ) | 0x30 );
    num_welds -= ( ( message[11] - 0x30 ) * 10000 );

    message[12] = ( ( num_welds / 1000 ) | 0x30 );
    num_welds -= ( ( message[12] - 0x30 ) * 1000 );

    message[13] = ( ( num_welds / 100 ) | 0x30 );
    num_welds -= ( ( message[13] - 0x30 ) * 100 );

    message[14] = ( ( num_welds / 10 ) | 0x30 );
    num_welds -= ( ( message[14] - 0x30 ) * 10 );

    message[15] = ( num_welds | 0x30 );

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)BLANK, LCD_COL_COUNT );
}



void LCD_create_info_screen_2( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    u32_t num_welds = NVM_info_s.NVM_generic_data_blk_s.total_num_welds;
    u8_t num;

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)SW_VERSION, LCD_COL_COUNT );

    num = VERSIONS_software_c.major_milestone;
    message[8] = ( ( num / 10 ) | 0x30 );
    num -= ( ( message[8] - 0x30 ) * 10 );
    message[9] = ( num | 0x30 );

    num = VERSIONS_software_c.patch;
    message[11] = ( ( num / 10 ) | 0x30 );
    num -= ( ( message[11] - 0x30 ) * 10 );
    message[12] = ( num | 0x30 );

    num = VERSIONS_software_c.verification;
    message[14] = ( ( num / 10 ) | 0x30 );
    num -= ( ( message[14] - 0x30 ) * 10 );
    message[15] = ( num | 0x30 );

    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)HW_VERSION, LCD_COL_COUNT );

    num = VERSIONS_hardware_c.major_milestone ;
    message[8] = ( ( num / 10 ) | 0x30 );
    num -= ( ( message[8] - 0x30 ) * 10 );
    message[9] = ( num | 0x30 );

    num = VERSIONS_software_c.patch;
    message[11] = ( ( num / 10 ) | 0x30 );
    num -= ( ( message[11] - 0x30 ) * 10 );
    message[12] = ( num | 0x30 );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)message, LCD_COL_COUNT );
}



void LCD_create_info_screen_3( void )
{
    /* Create a non const array to hold the LCD string */
    u8_t message[ LCD_COL_COUNT ] = { 0x00u, };
    s8_t num;
    u8_t temp;

    /* Now copy the const string into the new temp buffer */
    STDC_memcpy( (u8_t*)message, (u8_t*)TEMPERATURE, LCD_COL_COUNT );

    //TODO change to  eventually read internal temps
    num = MODE_MGR_read_internal_temp();

    if( num < 0 )
    {
        /* We are now negative so display a "-" sign */
        message[8] = 0x2D;
        num = abs(num);
    }

    message[9] = ( ( num / 100 ) | 0x30 );
    num -= ( ( message[9] - 0x30 ) * 100 );

    message[10] = ( ( num / 10 ) | 0x30 );
    num -= ( ( message[10] - 0x30 ) * 10 );

    message[11] = ( num | 0x30 );

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







void LCD_create_expert_mode_screen( void )
{
    /* Display the message and keep it on the screen until the timer expires */
    LCD_set_cursor_position(0,0);
    LCD_write_message( (u8_t*)EXPERT_MODE, LCD_COL_COUNT );

    LCD_set_cursor_position(1,0);
    LCD_write_message( (u8_t*)NO_COOLDOWN, LCD_COL_COUNT );
}

