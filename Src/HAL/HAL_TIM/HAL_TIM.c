#ifndef UNIT_TEST
    #include "stm32f10x_tim.h"
    #include "stm32f10x_exti.h"
    #include "misc.h"
#endif

#include "HAL_TIM.h"
#include "HAL_BRD.h"

u32_t HAL_TIM_seconds_ctr_s;
low_high_et state = LOW;


void HAL_TIM1_init( void )
{
	/* Enable the timer peripheral clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* TIM1 setup as a timer */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* set everything back to default values */
    TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 3600;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10-1u;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ITConfig( TIM1, TIM_IT_Update, ENABLE );
    TIM_Cmd(TIM1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Keep the interrupts disabled for now until needed */
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	HAL_TIM_seconds_ctr_s = 0u;
}


void HAL_TIM2_init(void)
{
	/* Enable the TIM2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE_);

	/* TIM2 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* set everything back to default values */
	TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);

	/* try and setup a 1ms period/count */
	TIM_TimeBaseStructure.TIM_Prescaler = 1800;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 200-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
	TIM_Cmd(TIM2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Keep the interrupts disabled for now until needed */
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}


void HAL_TIM3_init(void)
{
	/* Enable the TIM3 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE_);

	/* TIM3 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* set everything back to default values */
	TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);

	/* try and setup a 1ms period/count */
	TIM_TimeBaseStructure.TIM_Prescaler = 7200;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );
	TIM_Cmd(TIM3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Keep the interrupts disabled for now until needed */
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}




void HAL_TIM1_start( void )
{
	NVIC_InitTypeDef NVIC_InitStruct;

	TIM_SetCounter(TIM1, 0);

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}


void HAL_TIM1_stop( void )
{
	NVIC_InitTypeDef NVIC_InitStruct;

	TIM_SetCounter(TIM1, 0);

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	TIM_SetCounter(TIM1, 0);
}



void HAL_TIM2_start( void )
{
	NVIC_InitTypeDef NVIC_InitStruct;

	TIM_SetCounter(TIM2, 0);

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}



void HAL_TIM2_stop( void )
{
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	TIM_SetCounter(TIM2, 0);
}



void HAL_TIM3_start( void )
{
	NVIC_InitTypeDef NVIC_InitStruct;

	TIM_SetCounter(TIM3, 0);

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );
}




void HAL_TIM3_stop( void )
{
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Add IRQ vector to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

	TIM_SetCounter( TIM3, 0 );

	TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
	TIM_ClearFlag( TIM3, TIM_FLAG_Update );
	TIM_ITConfig( TIM3, TIM_IT_Update, DISABLE );
}



u32_t HAL_TIM_get_time( void )
{
	return ( HAL_TIM_seconds_ctr_s );
}



void HAL_TIM_increment_secs( void )
{
	if( HAL_TIM_seconds_ctr_s < U32_T_MAX )
    {
        HAL_TIM_seconds_ctr_s ++;
    }
}


void TIM1_UP_IRQHandler( void )
{
    if( TIM_GetITStatus( TIM1, TIM_IT_Update ) )
	{
    	TIM_ClearITPendingBit( TIM1, TIM_IT_Update );
	}
}


void TIM2_IRQHandler ( void )
{
	if( TIM_GetITStatus( TIM2, TIM_IT_Update ) )
	{
		TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
		HAL_TIM2_stop();

		/* The debounce timer has finished, execute the callback */
		HAL_BRD_debounce_completed();
	}
}


void TIM3_IRQHandler ( void )
{
    /* Spot weld timer Interrupt handler */
	if( TIM_GetITStatus( TIM3, TIM_IT_Update ) )
	{
		TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
	}
}
