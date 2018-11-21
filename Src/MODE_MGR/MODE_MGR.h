#ifndef MODE_MGR_H
#define MODE_MGR_H


#define MODE_MGR_MAX_TICK_CYCLE_VALUE 	1000u
#define MODE_MGR_TICK_RATE_MSECS		20u


typedef enum
{
    MODE_MGR_MODE_NORMAL = 0u,
    MODE_MGR_MODE_TEST,
    MODE_MGR_SETTINGS_MODE
} MODE_MGR_mode_et;



void MODE_MGR_init( void );
void MODE_MGR_20ms_tick( void );
void MODE_MGR_action_schedule_normal( void );
void MODE_MGR_change_mode( void );
void MODE_MGR_handle_settings( void );
MODE_MGR_mode_et MODE_MGR_get_mode( void );



#endif
