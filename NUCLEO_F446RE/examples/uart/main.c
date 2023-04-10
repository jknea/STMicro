#include "uart.h"

#define GPIOAEN		(1UL << 0)

#define LED_PIN		(1UL << 5);

uint8_t key;

int main()
{
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |=  (1UL << 10);
	GPIOA->MODER &= ~(1UL << 11);

	uart2_rxtx_init();
	while(1)
	{
		//printf("Hello from STM32F4!\r\n");
		//for (int i = 0; i < 1000000; i++) {}
		key = uart2_read();
		if (key == '1')
		{
			GPIOA->ODR |= LED_PIN;
		}
		else
		{
			GPIOA->ODR &= ~LED_PIN;
		}
//		for (int i = 0; i < 1000000; i++) {}
	}

	return 0;
}
