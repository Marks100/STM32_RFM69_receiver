/* STM32 specific includes */
#ifndef UNIT_TEST
	#include "misc.h"
    #include "stm32f10x.h"
    #include "stm32f10x_rcc.h"
    #include "stm32f10x_pwr.h"
	#include "stm32f10x_i2c.h"
	#include "stm32f10x_iwdg.h"
#endif

#include "STDC.h"
#include "HAL_BRD.h"
#include "HAL_ADC.h"
#include "HAL_SPI.h"
#include "HAL_I2C.h"
#include "HAL_TIM.h"
#include "MODE_MGR.h"
#include "RF_MGR.h"
#include "AIRCON.h"
#include "nvm.h"
#include "NEOPIXEL.h"
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

	NVM_init();

	/* Init the HW */
	HAL_BRD_init();
	HAL_SPI1_init();
	CLI_init();

	HAL_TIM2_init();

	/* Initialise the RF MGR */
	RF_MGR_init();

	MODE_MGR_init();

	AIRCON_init();

	NEOPIXEL_int();

	/* Init the systick timer */
	MAIN_SYSTICK_init();

	MAIN_WATCHDOG_init();

	while (1)
	{
	}
}




/* ISR for systick handler */
void SysTick_Handler( void )
{
	delay_timer--;

	debug_timer_s += 1u;
	HAL_BRD_toggle_onboard_led();

	MODE_MGR_tick();
}


u32_t get_counter( void )
{
	return ( ctr );
}


void MAIN_SYSTICK_init( void )
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);

	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK_Div8 );

	/* Trigger an interrupt every 1ms */
	SysTick_Config(3600000);
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



void MAIN_WATCHDOG_init( void )
{
	/* This pauses the watchdog while debugging */
	DBGMCU->CR |= DBGMCU_CR_DBG_IWDG_STOP;

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_4); // 4, 8, 16 ... 256
	IWDG_SetReload(0x07D0);//This parameter must be a number between 0 and 0x0FFF.
	IWDG_ReloadCounter();
	IWDG_Enable();
}


void MAIN_WATCHDOG_deinit( void )
{
	/* Cant disable :( */
}


void MAIN_WATCHDOG_kick( void )
{
	IWDG_SetReload(0x07D0);//This parameter must be a number between 0 and 0x0FFF.
	IWDG_ReloadCounter();
}



