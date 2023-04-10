/*
 * timer.c
 *
 *  Created on: Mar 22, 2023
 *      Author: kneaj
 */
#include "stm32f446inc/stm32f446xx.h"
#include "inc/timer.h"
#include "inc/gpio.h"
#include "inc/uart.h"

#include <stddef.h>


void timerToAltFuncAndChannel(uint8_t stm32_pin, uint8_t timer_num, int ret[2]);
void initPwm(TIM_TypeDef *TIMx, uint8_t CHx, uint16_t prescaler, uint32_t pwm_mode, uint32_t period, uint32_t duty_div);
void timerClockEnable(TIM_TypeDef * TIMx);
TIM_TypeDef * timerNumToPtr(uint8_t TIMn);

// GPIO HAL function declarations
void gpioSetAltFunc(uint8_t stm32_pin, uint32_t AFx);
GPIO_TypeDef * gpioPinToPortPtr(uint8_t stm32_pin);
void gpioClockEnable(GPIO_TypeDef * GPIOx);
int gpioPinNum(uint8_t stm32_pin);


void timerWaitForUif(TIM_TypeDef *TIMx)
{
	while (!(TIMx->SR & TIM_SR_UIF)) {}
}

void timerClearUif(TIM_TypeDef *TIMx)
{
	TIMx->SR &= ~TIM_SR_UIF;
}



void timerBasicInit(TIM_TypeDef *TIMx, uint16_t psc_val, uint16_t arr_val)
{
	timerClockEnable(TIMx);		// enable clk access to timer
	TIMx->PSC = psc_val;		// set prescaler value
	TIMx->ARR = arr_val;		// set aut-reload value
	TIMx->CNT = 0;				// clear counter
	TIMx->CR1 = TIM_CR1_CEN;	// enable timer
}



void timerOCInit_2to5_helper(TIM_TypeDef *TIMx, uint32_t CHx, uint32_t mode)
{
	switch (CHx)
	{
	case 1:
		TIMx->CCMR1 = (mode << 4); // set output compare toggle mode
		TIMx->CCER |= TIM_CCER_CC1E;	// enable TIMx chx in compare mode
		break;
	case 2:
		TIMx->CCMR1 = (mode << 12); // set output compare toggle mode
		TIMx->CCER |= TIM_CCER_CC2E;	// enable TIMx chx in compare mode
		break;
	case 3:
		TIMx->CCMR2 = (mode << 4); // set output compare toggle mode
		TIMx->CCER |= TIM_CCER_CC3E;	// enable TIMx chx in compare mode
		break;
	case 4:
		TIMx->CCMR2 = (mode << 12); // set output compare toggle mode
		TIMx->CCER |= TIM_CCER_CC4E;	// enable TIMx chx in compare mode
		break;
	}
}



void timerOCInit_2to5(uint8_t timer_num, uint16_t psc_val, uint16_t arr_val, uint8_t stm32_pin, uint32_t AFx, uint32_t CHx, uint32_t mode)
{
	TIM_TypeDef *TIMx = timerNumToPtr(timer_num);
	GPIO_TypeDef *GPIOx = gpioPinToPortPtr(stm32_pin);
	gpioClockEnable(GPIOx);
	pinMode(stm32_pin, ALTERNATE); // set pin to alternate mode
	gpioSetAltFunc(stm32_pin, AFx); 	// set alternate function to AFx
	timerClockEnable(TIMx);	// enable clock access to TIMx
	TIMx->PSC = psc_val;		// set prescaler value
	TIMx->ARR = arr_val;	// set auto-reload value
	timerOCInit_2to5_helper(TIMx, CHx, mode);
	TIMx->CNT = 0;	// clear counter
	TIMx->CR1 = TIM_CR1_CEN;	// enable timer
}


// ret is an array of ints: {AFx, CHx, M}
void timerToAltFuncAndChannel(uint8_t stm32_pin, uint8_t timer_num, int ret[2])
{
	switch(stm32_pin)
	{
		case PA5:
			switch(timer_num)
			{
				case 2:
					ret[0] = 1;
					ret[1] = 1;
					return;
			}
	}
}


void timerClockEnable(TIM_TypeDef * TIMx)
{
	if (TIMx == TIM1) { RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; return;}
	if (TIMx == TIM2) { RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; return;}
	if (TIMx == TIM3) { RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; return;}
	if (TIMx == TIM4) { RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; return;}
	if (TIMx == TIM5) { RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; return;}
	if (TIMx == TIM6) { RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; return;}
	if (TIMx == TIM7) { RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; return;}
	if (TIMx == TIM8) { RCC->APB2ENR |= RCC_APB2ENR_TIM8EN; return;}
	if (TIMx == TIM9) { RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; return;}
	if (TIMx == TIM10) { RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; return;}
	if (TIMx == TIM11) { RCC->APB2ENR |= RCC_APB2ENR_TIM11EN; return;}
	if (TIMx == TIM12) { RCC->APB1ENR |= RCC_APB1ENR_TIM12EN; return;}
	if (TIMx == TIM13) { RCC->APB1ENR |= RCC_APB1ENR_TIM13EN; return;}
	if (TIMx == TIM14) { RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; return;}
}

TIM_TypeDef * timerNumToPtr(uint8_t TIMx)
{
	switch (TIMx)
	{
		case 1: { return TIM1; }
		case 2: { return TIM2; }
		case 3: { return TIM3; }
		case 4: { return TIM4; }
		case 5: { return TIM5; }
		case 6: { return TIM6; }
		case 7: { return TIM7; }
		case 8: { return TIM8; }
		case 9: { return TIM9; }
		case 10: { return TIM10; }
		case 11: { return TIM11; }
		case 12: { return TIM12; }
		case 13: { return TIM13; }
		case 14: { return TIM14; }
	}
	return NULL;
}


void initPwmCh1_helper(TIM_TypeDef *TIMx, uint32_t pwm_mode, uint16_t duty_cycle)
{
	TIMx->CCMR1 = (pwm_mode << 4);	// set output compare PWM mode
	TIMx->CCER |= TIM_CCER_CC1E;	// enable TIM2 ch2 in compare mode
	TIMx->CCR1 = duty_cycle;	// set duty cycle
}

void initPwmCh2_helper(TIM_TypeDef *TIMx, uint32_t pwm_mode, uint32_t duty_cycle)
{
	TIMx->CCMR1 = (pwm_mode << 12);	// set output compare PWM mode
	TIMx->CCER |= TIM_CCER_CC2E;	// enable TIM2 ch1 in compare mode
	TIMx->CCR2 = duty_cycle;	// set duty cycle
}

void initPwmCh3_helper(TIM_TypeDef *TIMx, uint32_t pwm_mode, uint32_t duty_cycle)
{
	TIMx->CCMR2 = (pwm_mode << 4);	// set output compare PWM mode
	TIMx->CCER |= TIM_CCER_CC3E;	// enable TIM2 ch3 in compare mode
	TIMx->CCR3 = duty_cycle;	// set duty cycle
}

void initPwmCh4_helper(TIM_TypeDef *TIMx, uint32_t pwm_mode, uint32_t duty_cycle)
{
	TIMx->CCMR2 = (pwm_mode << 12);	// set output compare PWM mode
	TIMx->CCER |= TIM_CCER_CC4E;	// enable TIM2 ch3 in compare mode
	TIMx->CCR4 = duty_cycle;	// set duty cycle

}


void initPwm(TIM_TypeDef *TIMx, uint8_t CHx, uint16_t prescaler, uint32_t pwm_mode, uint32_t period, uint32_t duty_div)
{
	TIMx->PSC = prescaler;	// set prescaler value
	uint32_t ticks_per_sec = (SYS_FREQ / prescaler);
	uint32_t arr_value = (period * ticks_per_sec); // compute auto-reload value
	TIMx->ARR = arr_value;	// set auto-reload value
	uint32_t duty_cycle = arr_value / duty_div; // compute duty cycle
	switch(CHx)
	{
		case 1:
			initPwmCh1_helper(TIMx, pwm_mode, duty_cycle);
			break;
		case 2:
			initPwmCh2_helper(TIMx, pwm_mode, duty_cycle);
			break;
		case 3:
			initPwmCh3_helper(TIMx, pwm_mode, duty_cycle);
			break;
		case 4:
			initPwmCh4_helper(TIMx, pwm_mode, duty_cycle);
			break;
	}
	TIMx->CNT = 0;	// clear counter
	TIMx->CR1 = TIM_CR1_CEN;	// enable timer
}





void timx_pwm(uint8_t pin_name, uint8_t tim_num, uint32_t pwm_mode, uint16_t prescaler, float period, float duty)
{
	GPIO_TypeDef *GPIOx = gpioPinToPortPtr(pin_name);
	gpioClockEnable(GPIOx);
	pinMode(pin_name, ALTERNATE);
	int AFx_CHx[2];
	timerToAltFuncAndChannel(pin_name, tim_num, AFx_CHx);
	int AFx = AFx_CHx[0];
	int CHx = AFx_CHx[1];
	gpioSetAltFunc(pin_name, AFx);
	TIM_TypeDef *TIMx = timerNumToPtr(tim_num);
	timerClockEnable(TIMx);
	initPwm(TIMx, CHx, prescaler, pwm_mode, period, duty);
}
