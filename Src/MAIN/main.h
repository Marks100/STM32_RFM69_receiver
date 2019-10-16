#ifndef MAIN_H
#define MAIN_H

#include "c_defs.h"

void  delay_ms(u16_t);
void  delay_us(u16_t us);
u32_t get_counter( void );
void  MAIN_SYSTICK_init( void );


#endif
