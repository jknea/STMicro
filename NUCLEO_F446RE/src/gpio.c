/*! @file gpio.c
 *  @brief Provides the implementation for STM32F446 GPIO pins
 *
 *  Details.
 */

#include "stm32f446inc/stm32f446xx.h"
#include "inc/gpio.h"

/* functions hidden to user */
uint8_t gpioPinNum(uint8_t stm32_pin);
GPIO_TypeDef * gpioPinToPort(uint8_t stm32_pin);


GPIO_TypeDef * gpio_ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH};
uint8_t arduino_digitals[] = {D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15};


void allPinMode(pinmode_t mode)
{
	for (int i = 0; i < 14; i++)
	{
		pinMode(arduino_digitals[i], mode);
	}
}

void allDigitalWrite(logic_t logic)
{
	for (int i = 0; i < 14; i++)
	{
		digitalWrite(arduino_digitals[i], logic);
	}
}


void gpioClockEnable(pin_t stm32_pin)
{
	uint8_t port = stm32_pin / 16;
	RCC->AHB1ENR |= (1UL << port);
}


void gpioSetAltFunc(pin_t stm32_pin, uint32_t AFx)
{
	GPIO_TypeDef *GPIOx = gpioPinToPort(stm32_pin);
	uint8_t pnum = gpioPinNum(stm32_pin);
	uint8_t AFRn = (pnum / 8);
	uint8_t shift = (pnum % 8) * 4;
	GPIOx->AFR[AFRn] |= (AFx << shift);
}


GPIO_TypeDef * gpioPinToPort(pin_t stm32_pin)
{
	return gpio_ports[stm32_pin / 16];  // will return 0 for port A, 1 for port b
}


/* Set the pin mode of an STM32 GPIO PIN. Modes are : {INPUT, OUTPUT, ALTERNATE, ANALOG} */
void pinMode(pin_t stm32_pin, pinmode_t mode)
{
	gpioClockEnable(stm32_pin);
	GPIO_TypeDef * GPIOx = gpioPinToPort(stm32_pin);
	uint8_t pin_num = gpioPinNum(stm32_pin);
	uint32_t lowbit = mode & BIT0_MASK;
	uint32_t highbit = mode & BIT1_MASK;
	if (lowbit == 0) { GPIOx->MODER &= ~(1UL << (2*pin_num)); }
	else { GPIOx->MODER |= (1UL << (2*pin_num)); }
	if (highbit == 0) { GPIOx->MODER &= ~(1UL << (2*pin_num + 1)); }
	else { GPIOx->MODER |= (1UL << (2*pin_num + 1)); }
}


uint8_t gpioPinNum(pin_t stm32_pin)
{
	return stm32_pin % 16;
}


void digitalWrite(pin_t stm32_pin, logic_t logic)
{
	GPIO_TypeDef *GPIOx = gpioPinToPort(stm32_pin);
	uint8_t pin_num = gpioPinNum(stm32_pin);
	if (logic == HIGH) { GPIOx->ODR |= (1UL << pin_num); }
	else { GPIOx->ODR &= ~(1UL << pin_num); }
}
