/*
 * systick.c
 *
 *  Created on: Mar 22, 2023
 *      Author: kneaj
 */


#include "stm32f446inc/stm32f446xx.h"
#include "stm32f446inc/core_cm4.h"

#define SYS_FREQ		16000000
#define SYSTICK_16MHz_1ms		(SYS_FREQ/1000) - 1
#define SYSTICK_16MHz_1us		(SYS_FREQ/1000000) - 1

void delay_ms(uint32_t delay_ms)
{
	SysTick->LOAD = SYSTICK_16MHz_1ms;	// reload with number of clocks per millisecond
	SysTick->VAL = 0;	// clear current value register
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);	// enable systick and select internal clock source
	for (int i = 0; i < delay_ms; i++)
	{
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}		// wait until the countflag is set
	}
	SysTick->CTRL = 0;
}

void delay_us(uint32_t delay_us)
{
	SysTick->LOAD = SYSTICK_16MHz_1us;	// reload with number of clocks per millisecond
	SysTick->VAL = 0;	// clear current value register
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);	// enable systick and select internal clock source
	for (int i = 0; i < delay_us; i++)
	{
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}		// wait until the countflag is set
	}
	SysTick->CTRL = 0;
}
