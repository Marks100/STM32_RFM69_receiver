/* STM32 specific includes */
#ifndef UNIT_TEST
	#include "misc.h"
    #include "stm32f10x.h"
    #include "stm32f10x_rcc.h"
#endif

#include "STDC.h"
#include "HAL_BRD.h"
#include "HAL_ADC.h"
#include "HAL_SPI.h"
#include "HAL_I2C.h"
#include "HAL_TIM.h"
#include "HAL_UART.h"
#include "MODE_MGR.h"
#include "BTN_MGR.h"
#include "RF_MGR.h"
#include "AIRCON.h"
#include "NVM.h"
#include "LCD.h"
#include "TEMP.h"
#include "HAL_BRD.h"
#include "NEOPIXEL.h"
#include "WDG.h"
#include "main.h"


int main(void)
{
    RCC_DeInit();
	SystemInit();

	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	NVM_init();

	/* Init the HW drivers */
	HAL_BRD_init();
	HAL_SPI1_init();
	HAL_I2C_init();
	HAL_TIM2_init();
    HAL_USART2_init();

	/* Initialise the SW modules */
	RF_MGR_init();
	MODE_MGR_init();
	AIRCON_init();
	//NEOPIXEL_int();
	BTN_MGR_init();
	TEMP_init();
	CLI_init();
	ROTARY_init();
	LCD_init();

	/* Init the systick timer */
	MAIN_SYSTICK_init();

	/* Init the WDg right at the end */
	//WDG_init();

	while (1)
	{
	}
}


/* ISR for systick handler */
void SysTick_Handler( void )
{
	MODE_MGR_tick();
}



void MAIN_SYSTICK_init( void )
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);

	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK_Div8 );

	/* Trigger an interrupt every 1ms */
	SysTick_Config(3600000);
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






