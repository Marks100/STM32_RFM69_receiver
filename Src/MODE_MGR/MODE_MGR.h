#ifndef MODE_MGR_H
#define MODE_MGR_H


#define MODE_MGR_MAX_TICK_CYCLE_VALUE 	1000u
#define MODE_MGR_TICK_RATE_MSECS		20u

#define COMMAND_1  ( 0 )
#define COMMAND_2  ( 1 )
#define COMMAND_3  ( 2 )
#define COMMAND_4  ( 3 )
#define COMMAND_5  ( 4 )
#define COMMAND_6  ( 5 )
#define COMMAND_7  ( 6 )
#define COMMAND_8  ( 7 )
#define COMMAND_9  ( 8 )
#define COMMAND_10 ( 9 )
#define COMMAND_11 ( 10 )
#define COMMAND_12 ( 11 )
#define COMMAND_13 ( 12 )
#define COMMAND_14 ( 13 )
#define COMMAND_15 ( 14 )
#define COMMAND_16 ( 15 )


typedef enum
{
    MODE_MGR_MODE_NORMAL = 0u,
    MODE_MGR_MODE_TEST,
    MODE_MGR_SETTINGS_MODE
} MODE_MGR_mode_et;



void MODE_MGR_init( void );
void MODE_MGR_20ms_tick( void );
void MODE_MGR_action_schedule_normal( void );
void MODE_MGR_analyse_switches( void );
void MODE_MGR_change_mode( void );
void MODE_MGR_handle_settings( void );
MODE_MGR_mode_et MODE_MGR_get_mode( void );
void MODE_MGR_action_selector_switch_changes( u8_t MODE_MGR_selector_switch_state_s );



#endif
