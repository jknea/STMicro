/*
 * gpio.c
 *
 *  Created on: Mar 20, 2023
 *      Author: kneaj
 */

#include "stm32f446inc/stm32f446xx.h"
#include "inc/gpio.h"
#include <stddef.h>

uint8_t gpioPinNum(uint8_t stm32_pin);
GPIO_TypeDef * gpioPinToPort(uint8_t stm32_pin);
GPIO_TypeDef * gpio_ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH};

void gpioClockEnable(GPIO_TypeDef * GPIOx)
{
	if (GPIOx == GPIOA) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; return; }
	if (GPIOx == GPIOB) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; return; }
	if (GPIOx == GPIOC) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; return; }
	if (GPIOx == GPIOD) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; return; }
	if (GPIOx == GPIOE) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; return; }
	if (GPIOx == GPIOF) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; return; }
	if (GPIOx == GPIOG) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; return; }
	if (GPIOx == GPIOH) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; return; }
}



void gpioSetAltFunc(uint8_t stm32_pin, uint32_t AFx)
{
	GPIO_TypeDef *GPIOx = gpioPinToPort(stm32_pin);
	uint8_t pnum = gpioPinNum(stm32_pin);
	uint8_t AFRn = (pnum / 8);
	uint8_t shift = (pnum % 8) * 4;
	GPIOx->AFR[AFRn] |= (AFx << shift);
}



GPIO_TypeDef * gpioPinToPort(uint8_t stm32_pin)
{
	return gpio_ports[stm32_pin / 16];  // will return 0 for port A, 
}


/*
 * Set the pin mode of an STM32 GPIO PIN
 * Modes are : {INPUT, OUTPUT, ALTERNATE, ANALOG}
 */
void pinMode(uint8_t stm32_pin, uint8_t mode)
{
	GPIO_TypeDef * GPIOx = gpioPinToPort(stm32_pin);
	gpioClockEnable(GPIOx);
	uint8_t pin_num = gpioPinNum(stm32_pin);
	uint32_t lowbit = mode & BIT0_MASK;
	uint32_t highbit = mode & BIT1_MASK;
	if (lowbit == 0) { GPIOx->MODER &= ~(1UL << (2*pin_num)); }
	else { GPIOx->MODER |= (1UL << (2*pin_num)); }

	if (highbit == 0) { GPIOx->MODER &= ~(1UL << (2*pin_num + 1)); }
	else { GPIOx->MODER |= (1UL << (2*pin_num + 1)); }
}



uint8_t gpioPinNum(uint8_t stm32_pin)
{
	return stm32_pin % 16;
}



void digitalWrite(uint8_t stm32_pin, uint8_t mode)
{
	GPIO_TypeDef *GPIOx = gpioPinToPort(stm32_pin);
	uint8_t pin_num = gpioPinNum(stm32_pin);
	if (mode == HIGH) { GPIOx->ODR |= (1UL << pin_num); }
	else { GPIOx->ODR &= ~(1UL << pin_num); }
}
