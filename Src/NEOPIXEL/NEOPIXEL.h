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

#define NEOPIXEL_ALL_LED 0xFF

#define NEOPIXEL_LED_MAX	12u
#define NEOPIXEL_CMD_BITS	24u

#define NEOPIXEL_COLOUR_NONE 0u

#define BV(x) 			(1u<<(x))

typedef enum
{
	NEOPIXEL_NO_STATE = 0u,
	NEOPIXEL_CENTRE_SWEEP,
	NEOPIXEL_ALL_FLASH,
	NEOPIXEL_CIRCLE_BUFFER

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
	u8_t 				 stage;
} NEOPIXEL_data_st;

void NEOPIXEL_set_led( u32_t led_pos, u32_t colour );
void NEOPIXEL_clear_all_leds( void );
void NEOPIXEL_light_all_leds( u32_t colour );
void NEOPIXEL_tick( void );
NEOPIXEL_ll_state_et NEOPIXEL_handle_centre_sweep( u8_t* stage );
NEOPIXEL_ll_state_et NEOPIXEL_handle_all_flash( u8_t* stage );
NEOPIXEL_ll_state_et NEOPIXEL_handle_circle_buffer( u8_t* stage );
void NEOPIXEL_set_hl_state( NEOPIXEL_hl_state_et state );



#endif
