#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include "HAL_BRD.h"

#define NEOPIXEL_LED1	0
#define NEOPIXEL_LED2	1
#define NEOPIXEL_LED3	2
#define NEOPIXEL_LED4	3
#define NEOPIXEL_LED5	4
#define NEOPIXEL_LED6	5
#define NEOPIXEL_LED7	6
#define NEOPIXEL_LED8	7
#define NEOPIXEL_LED9	8
#define NEOPIXEL_LED10	9
#define NEOPIXEL_LED11	10
#define NEOPIXEL_LED12	11

#define NEOPIXEL_ALL_LED 0xFFFFFFFF

#define NEOPIXEL_LED_MAX	12u
#define NEOPIXEL_CMD_BITS	24u

#define NEOPIXEL_COLOUR_NONE 0u
#define NEOPIXEL_WHITE	 0xFFFFFF
#define NEOPIXEL_RED	 0x00FF00
#define NEOPIXEL_LIME	 0xFF0000
#define NEOPIXEL_BLUE	 0x0000FF
#define NEOPIXEL_YELLOW	 0xFFFF00
#define NEOPIXEL_ORANGE	 0x47ff00
#define NEOPIXEL_CYAN	 0xFF00FF
#define NEOPIXEL_MAGENTA 0x00FFFF
#define NEOPIXEL_SILVER	 0xC0C0C0
#define NEOPIXEL_GRAY	 0x808080
#define NEOPIXEL_MAROON	 0x008000
#define NEOPIXEL_OLIVE	 0x808000
#define NEOPIXEL_GREEN	 0x800000
#define NEOPIXEL_PURPLE	 0x008080
#define NEOPIXEL_TEAL	 0x800080
#define NEOPIXEL_NAVY	 0x000080

#define BV(x) 			(1u<<(x))

typedef enum
{
	NEOPIXEL_NO_STATE = 0u,
	NEOPIXEL_CENTRE_SWEEP,
	NEOPIXEL_ALL_FLASH,
	NEOPIXEL_CIRCLE_BUFFER,
	NEOPIXEL_FLOW,
	NEOPIXEL_TEMPERATURE,
	NEOPIXEL_INDICATOR,
	NEOPIXEL_MANUAL
} NEOPIXEL_hl_state_et;

typedef enum
{
	NEOPIXEL_IDLE = 0u,
	NEOPIXEL_RUNNING,
	NEOPIXEL_FINISHED,
} NEOPIXEL_ll_state_et;

typedef struct
{
	NEOPIXEL_hl_state_et hl_state;
	NEOPIXEL_ll_state_et ll_state;
	u8_t 				 ticks;
	u8_t 				 led_num;
	u8_t 				 state_num;
} NEOPIXEL_data_st;

void NEOPIXEL_int( void );
void NEOPIXEL_set_led( u32_t led_pos, u32_t colour );
void NEOPIXEL_shiftt_led( u32_t colour );
void NEOPIXEL_clear_all_leds( void );
void NEOPIXEL_light_all_leds( u32_t colour );
void NEOPIXEL_tick( void );
NEOPIXEL_ll_state_et NEOPIXEL_handle_centre_sweep( u8_t* stage );
NEOPIXEL_ll_state_et NEOPIXEL_handle_all_flash( u8_t* stage );
NEOPIXEL_ll_state_et NEOPIXEL_handle_circle_buffer( u8_t* ticks );
NEOPIXEL_ll_state_et NEOPIXEL_handle_flow( u8_t* ticks );
NEOPIXEL_ll_state_et NEOPIXEL_handle_temperature_control( u8_t* ticks );
NEOPIXEL_ll_state_et NEOPIXEL_handle_audi_indicator( u8_t* ticks );
NEOPIXEL_ll_state_et NEOPIXEL_handle_manual( u8_t* ticks );
void NEOPIXEL_set_hl_state( NEOPIXEL_hl_state_et state );
void NEOPIXEL_handle_rotary_input( ROTARY_scroll_type_et scroll );



#endif
