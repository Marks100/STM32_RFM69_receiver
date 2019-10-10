#ifndef MODE_MGR_H
#define MODE_MGR_H

#include "HAL_BRD.h"

#define MODE_MGR_MAX_TICK_CYCLE_VALUE 	1000u
#define MODE_MGR_TICK_RATE_MSECS		50u
#define MODE_MGR_NUM_SLIDER_INPUTS		4u
#define MODE_MGR_SLIDER_DB_TICKS		5u

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
	MODE_MGR_MODE_STARTUP = 0u,
	MODE_MGR_MODE_WAIT,
    MODE_MGR_MODE_NORMAL,
    MODE_MGR_MODE_TEST,
    MODE_MGR_SETTINGS_MODE
} MODE_MGR_mode_et;


typedef struct
{
	low_high_et slider_state_ll;
	low_high_et slider_state_hl;
	u8_t slider_debounce_time;
} MODE_MGR_user_input_st;


void MODE_MGR_init( void );
void MODE_MGR_tick( void );
void MODE_MGR_action_schedule_normal( void );
void MODE_MGR_action_schedule_test( void );
void MODE_MGR_check_user_input( void );
void MODE_MGR_change_mode( MODE_MGR_mode_et mode );
void MODE_MGR_handle_settings( void );
MODE_MGR_mode_et MODE_MGR_get_mode( void );
void MODE_MGR_action_selector_switch_changes( HAL_BRD_switch_slider_et slider, low_high_et state );




#endif
