/* STM32 specific includes */
#ifndef UNIT_TEST
	#include "misc.h"
    #include "stm32f10x.h"
    #include "stm32f10x_rcc.h"
    #include "stm32f10x_pwr.h"
	#include "stm32f10x_i2c.h"
#endif

#include "C_defs.h"
#include "PROJ_config.h"
#include "COMPILER_defs.h"
#include "COMPILER_config.h"
#include "HAL_BRD.h"
#include "HAL_ADC.h"
#include "HAL_SPI.h"
#include "HAL_I2C.h"
#include "HAL_UART.h"
#include "HAL_TIM.h"
#include "RFM69.h"
#include "main.h"


u16_t delay_timer;
u32_t ctr;
u16_t debug_timer_s;

int main(void)
{
 	ctr = 0;
	delay_timer = 0u;
	debug_timer_s = 0u;

	RCC_DeInit();
	SystemInit();

	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Init the HW */
	HAL_BRD_init();
	HAL_I2C_init();
	HAL_SPI_init();
	NVM_init();
	HAL_TIM_1_init();
    /* Start the timer to keep track of reception count */
	HAL_TIM_1_start();

	/* Initialise the RFM69 variables */
	RFM69_init();

	/* Init the systick timer */
	MAIN_SYSTICK_init();

	if( debug_mode == ENABLE )
	{
		/* In debug mode lets init the debug usart as this consumes lots of power */
		SERIAL_init();

		/* power up the RF chip */
		RFM69_set_enable_pin_state( HIGH );
		RFM69_set_reset_pin_state( LOW );
	}

	RFM69_setup_receive_mode();

	while (1)
	{
		/* Handle the serial messages */
		SERIAL_msg_handler();
		RFM69_receive_frame();
	}
}




/* ISR for systick handler */
void SysTick_Handler( void )
{
	delay_timer--;

	debug_timer_s += 1u;

	if( debug_timer_s >= 1000u )
	{
		/* reset the task timer */
		debug_timer_s = 0u;

		/* Trigger the stream */
		SERIAL_trigger_stream_output();
	}
}


u32_t get_counter( void )
{
	return ( ctr );
}


void MAIN_SYSTICK_init( void )
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);

	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	/* Trigger an interrupt every 1ms */
	SysTick_Config(72000);
}


void delay_ms(u16_t ms)
{
	delay_timer = ms;
	while(delay_timer != 0);
}



void delay_us(u16_t us)
{
	asm volatile (	"MOV R0,%[loops]\n\t"\
			"1: \n\t"\
			"SUB R0, #1\n\t"\
			"CMP R0, #0\n\t"\
			"BNE 1b \n\t" : : [loops] "r" (10*us) : "memory"\
		      );
}





