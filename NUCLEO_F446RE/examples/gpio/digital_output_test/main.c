/*
 ******************************************************************************
 * @file           : main.c
 * @author         : jknea
 * @brief          : Main program body
 ******************************************************************************
 *
 * This program is to test the output capabilities of pins D2-D15.  Sending the
 * number 1 through Realterm turns all pins HIGH, and sending a non-one turns
 * all pins low.  An led is connected to a 10k resistor for testing.
 *
 * Note:
 * D0 == PA3	(CANT USE SINCE NEEDED FOR USART2_RX which is tied to USB)
 * D1 == PA2	(CANT USE SINCE NEEDED FOR USART2_TX which is tied to USB)
 *
 ******************************************************************************
 */

#include "inc/gpio.h"
#include "inc/uart.h"

uint8_t key;

int main()
{
	uart2_txrx_init(115200);
	allPinMode(OUTPUT);
	while(1)
	{
		key = uart2_read();
		if (key == 1)
		{
			allDigitalWrite(HIGH);
		}
		else
		{
			allDigitalWrite(LOW);
		}
	}
	return 0;
}
