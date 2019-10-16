#ifndef UNIT_TEST
    #include "stm32f10x_adc.h"
    #include "stm32f10x_rcc.h"
#endif

#include "C_defs.h"
#include "STDC.h"
#include "HAL_ADC.h"

u16_t adc_result_s;



void HAL_ADC_de_init( void )
{
    /* Reset the ADC module  */
	ADC_DeInit( ADC1 );

	// disable ADC system clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
}


u16_t HAL_ADC_sample_batt_voltage( void )
{
	adc_result_s = 0u;
	u16_t batt_voltage = 0u;

	/* First thing to do is turn on the battery voltage feature by enabling the GPIO pin */
	//HAL_BRD_Set_batt_monitor_state( ENABLE );

	//clock for ADC (max 14MHz --> 72/6=12MHz)
	RCC_ADCCLKConfig (RCC_PCLK2_Div6);

	// enable ADC system clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	// define ADC config
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	// we work in continuous sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); // define regular conversion config
	ADC_Init (ADC1, &ADC_InitStructure);	//set config of ADC1

	// Enable ADC
	ADC_Cmd (ADC1, ENABLE);	//enable ADC1

	//	ADC calibration (optional, but recommended at power on)
	ADC_ResetCalibration(ADC1);	// Reset previous calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
	while(ADC_GetCalibrationStatus(ADC1));

	// start conversion
	ADC_Cmd (ADC1,ENABLE);	//enable ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion

	/* Wait while the conversion completes */
	while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 );

	adc_result_s = ADC_GetConversionValue(ADC1);

	batt_voltage = ( ADC_MAX_COUNT/ adc_result_s * ADC_BANDGAP_VOLTAGE );

	/* Deinit the ADC again to save power */
	HAL_ADC_de_init();

	/* Disable the battery voltage feature by disabling the GPIO pin to save power */
	//HAL_BRD_Set_batt_monitor_state( DISABLE );

	return ( batt_voltage );
}

