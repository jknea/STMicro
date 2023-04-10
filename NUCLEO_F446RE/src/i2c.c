/*
 * i2c.c
 *
 *  Created on: Mar 26, 2023
 *      Author: kneaj
 */
#include "stm32f446inc/stm32f446xx.h"

//pb8 = SCL1
//pb0 == SDA

#include "inc/gpio.h"

#define I2C_100KHZ	80
#define SD_MODE_MAX_RISE_TIME 	17

void gpioClockEnable(GPIO_TypeDef * GPIOx);
void gpioSetAltFunc(uint8_t stm32_pin, uint32_t AFx);

void i2c1Init()
{
	gpioClockEnable(GPIOB);	// enable clock access to i2c1

	// set pb8 and pb9 alt func
	pinMode(PB8, ALTERNATE);
	gpioSetAltFunc(PB8, 4);
	pinMode(PB9, ALTERNATE);
	gpioSetAltFunc(PB9, 4);

	// set pb8 and pb9 output to open drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT8;
	GPIOB->OTYPER |= GPIO_OTYPER_OT9;

	// enabble pb8 and pb9 pullup resistors
	GPIOB->PUPDR |= (1UL << 16);
	GPIOB->PUPDR &= ~(1UL << 17);

	GPIOB->PUPDR |= (1UL << 18);
	GPIOB->PUPDR &= ~(1UL << 19);

	// enable i2c clock
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	// first, enter reset mode
	I2C1->CR1 |= I2C_CR1_SWRST;
	//come out of reset
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	//set bus frequency
	I2C1->CR2 = 16;	// number in MHz

	//set i2c mode
	I2C1->CCR = I2C_100KHZ;

	// set rise time
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	// enable
	I2C1->CR1 |= I2C_CR1_PE;
}


void i2c1_byteRead(uint8_t saddr, uint8_t maddr, uint8_t *data)
{
	while (I2C1->SR2 & I2C_SR2_BUSY) {} 	// wait until bus is not busy
	I2C1->CR1 |= I2C_CR1_START; 			// generate start
	while (!(I2C1->SR1 & I2C_SR1_SB)) {}	// wait for start bit to be set
	I2C1->DR = saddr << 1; 					// TRANSMIT SLAVE ADDRESS + WRITE
	while (!(I2C1->SR1 & I2C_SR1_ADDR)) {} 	// wait until addr flag is set
	I2C1->SR2; 								// clear the addr flag
	I2C1->DR = maddr;						// send the memory address of register to read
	while (!(I2C1->SR1 & I2C_SR1_TXE)) {} 	// WAIT UNTIL TRANSMITTER EMPTY;
	I2C1->CR1 |= I2C_CR1_START; 			// generate start
	while (!(I2C1->SR1 & I2C_SR1_SB)) {}	// wait for start bit to be set
	// READ FROM SLAVE
	I2C1->DR = (saddr << 1) | 1; 			// TRANSMIT SLAVE ADDRESS + read
	while (!(I2C1->SR1& I2C_SR1_ADDR)) {} 	// wait until addr flag is set
	I2C1->CR1 &= ~I2C_CR1_ACK; 				// DISABLE ACK
	I2C1->SR2; 								// clear the addr flag by reading
	I2C1->CR1 |= I2C_CR1_STOP; 				// generate top
	while (!(I2C1->SR1 & I2C_SR1_RXNE)) {} 	// wait until RXNE flag is set
	*data = I2C1->DR;
}

void i2c1_burstRead(uint8_t saddr, uint8_t maddr, int n, uint8_t *data)
{
//	volatile int tmp;

	while (I2C1->SR2 & I2C_SR2_BUSY) {} 	// wait until bus not busy
	I2C1->CR1 |= I2C_CR1_START; 			// generate start
	while (!(I2C1->SR1 & I2C_SR1_SB)) {}	// wait for start bit to be set
	I2C1->DR = saddr << 1; 					// TRANSMIT SLAVE ADDRESS + WRITE
	while (!(I2C1->SR1& I2C_SR1_ADDR)) {} 	// wait until addr flag is set
	I2C1->SR2; 								// clear the addr flag by reading
	while (!(I2C1->SR1 & I2C_SR1_TXE)) {} 	// WAIT UNTIL TRANSMITTER EMPTY;
	I2C1->DR = maddr;						// send the memory address of register to read
	while (!(I2C1->SR1 & I2C_SR1_TXE)) {} 	// WAIT UNTIL TRANSMITTER EMPTY;
	I2C1->CR1 |= I2C_CR1_START; 			// generate start
	while (!(I2C1->SR1 & I2C_SR1_SB)) {}	// wait for start bit to be set
	// READ FROM SLAVE
	I2C1->DR = (saddr << 1) | 1; 			// TRANSMIT SLAVE ADDRESS + read
	while (!(I2C1->SR1 & I2C_SR1_ADDR)) {} 	// wait until addr flag is set
	I2C1->SR2; 								// clear the addr flag
	I2C1->CR1 |= I2C_CR1_ACK;

	while (n > 0)
	{
		if (n == 1)
		{
			I2C1->CR1 &= ~I2C_CR1_ACK; 		// DISABLE ACK
			I2C1->CR1 |= I2C_CR1_STOP;     	// generate stop
			break;
		}
		while (!(I2C1->SR1 & I2C_SR1_RXNE)) {} // wait until RXNE flag is set
		*data++ = I2C1->DR;
		n--;
	}
}

void i2c1_burstWrite(uint8_t saddr, uint8_t maddr, int n, uint8_t *data)
{
	//volatile int tmp;
	while (I2C1->SR2 & I2C_SR2_BUSY) {} 	// wait until not busy
	I2C1->CR1 |= I2C_CR1_START; 			// generate start
	while (!(I2C1->SR1 & I2C_SR1_SB)) {}	// wait for start bit to be set
	I2C1->DR = saddr << 1; // TRANSMIT SLAVE ADDRESS + WRITE
	while (!(I2C1->SR1 & I2C_SR1_ADDR)) {} 	// wait until addr flag is set
	I2C1->SR2; // clear the addr flag
	while (!(I2C1->SR1 & I2C_SR1_TXE)) {} // WAIT data register EMPTY;
	I2C1->DR = maddr;	// send the memory address of register to read
	for (int i = 0; i < n; i++)
	{
		while (!(I2C1->SR1 & I2C_SR1_TXE)) {} // WAIT data register EMPTY;
		I2C1->DR = *data++;
	}
	while (!(I2C1->SR1 & I2C_SR1_BTF)) {}
	I2C1->CR1 |= I2C_CR1_STOP; // generate top
}
