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

#define NEOPIXEL_LED_MAX	12u
#define NEOPIXEL_CMD_BITS	24u

#define NEOPIXEL_COLOUR_NONE 0u

#define BV(x) 			(1u<<x)

void NEOPIXEL_set_led( u32_t led_pos, u32_t colour );
void NEOPIXEL_clear_all_leds( void );
void NEOPIXEL_light_all_leds( u32_t colour );



#endif
