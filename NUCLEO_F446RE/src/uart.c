/*
 * uart.c
 *
 *  Created on: Mar 20, 2023
 *      Author: kneaj
 */
#include "uart.h"
#include "gpio.h"
#include "stm32f446xx.h"

#define SYS_FREQ	16000000UL	// internal RC oscillator frequency
#define APB1_CLK	SYS_FREQ
#define	UART_BAUD	115200


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphClk, uint32_t baudrate);
static uint16_t uart_compute_div(uint32_t periphClk, uint32_t baudrate);

void uart2_tx_init();
void uart2_write(int ch);


// GPIO HAL function declarations
void gpioSetAltFunc(uint8_t stm32_pin, uint32_t AFx);
GPIO_TypeDef * gpioPinToPortPtr(uint8_t stm32_pin);
void gpioClockEnable(GPIO_TypeDef * GPIOx);
int gpioPinNum(uint8_t stm32_pin);



// re-target printf to use uart2_write
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

int __io_getchar()
{
	return uart2_read();
}

/*
void uart2_txrx_init()
{
	// CONFIGURE UART GPIO PIN
	// 1. enable clk to GPIOA
	gpioClockEnable(GPIOA);

	// set PA2 mode to alternate function through MODER register
	pinMode(PA2, ALTERNATE);
	gpioSetAltFunc(PA2, 7);	// set PA2 alternate function time to UART_TX (AFZ=7)

	// set PA3 mode to alternate function through MODER register
	pinMode(PA3, ALTERNATE);
	gpioSetAltFunc(PA3, 7);	// set PA3 alternate function time to UART_TX (AFZ)


	// CONGFIGURE UART MODULE
	// 1. enable clock access to uart2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUD);

	// configure transfer didrection
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;


	// enable uart module
	USART2->CR1 |= USART_CR1_UE;

}
*/


void uart2_txrx_init()
{
	// CONFIGURE UART GPIO PIN
	// 1. enable clk to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA2 mode to alternate function through MODER register
	//GPIOA->MODER &= ~(1UL << 4);
	//GPIOA->MODER |=  (1UL << 5);
	pinMode(PA2, ALTERNATE);

	// set PA2 alternate function time to UART_TX (AFZ=7)
//	GPIOA->AFR[0] |=  (1UL << 8);
//	GPIOA->AFR[0] |=  (1UL << 9);
//	GPIOA->AFR[0] |=  (1UL << 10);
//	GPIOA->AFR[0] &= ~(1UL << 11);
//	GPIOA->AFR[0] |= (7UL << 8);
	gpioSetAltFunc(PA2, 7UL);	// set PA2 alternate function time to UART_TX (AFZ=7)


	// set PA3 mode to alternate function through MODER register
//	GPIOA->MODER &= ~(1UL << 6);
//	GPIOA->MODER |=  (1UL << 7);
	pinMode(PA3, ALTERNATE);

	// set PA3 alternate function time to UART_TX (AFZ)
//	GPIOA->AFR[0] |=  (1UL << 12);
//	GPIOA->AFR[0] |=  (1UL << 13);
//	GPIOA->AFR[0] |=  (1UL << 14);
//	GPIOA->AFR[0] &= ~(1UL << 15);
	gpioSetAltFunc(PA3, 7UL);
//	GPIOA->AFR[0] |= (7UL << 12);


	// CONGFIGURE UART MODULE
	// 1. enable clock access to uart2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUD);

	// configure transfer didrection
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;


	// enable uart module
	USART2->CR1 |= USART_CR1_UE;

	printf("Usart2 init success!\r\n");
}



void uart2_write(int ch)
{
	// 1. make sure transmit data register is empty
	while (!(USART2->SR & USART_SR_TXE)) {}
	USART2->DR = (ch & 0xFF);	// send 8-bits of data
}

char uart2_read()
{
	while (!(USART2->SR & USART_SR_RXNE)) {}
	return USART2->DR;
}




static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphClk, uint32_t baudrate)
{
	USARTx->BRR = uart_compute_div(periphClk, baudrate);
}

static uint16_t uart_compute_div(uint32_t periphClk, uint32_t baudrate)
{
	return ((periphClk + baudrate/2U) / baudrate);
}
