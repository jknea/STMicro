#include "gpio.h"
#include "uart.h"

#define	UART_BAUD	115200

#define USER_LED	D13

uint8_t key;

// D0 == PA3	(CANT USE SINCE NEEDED FOR USART2_RX which is tied to USB)
// D1 == PA2	(CANT USE SINCE NEEDED FOR USART2_TX which is tied to USB)


int main()
{

	pinMode(USER_LED, OUTPUT);

	uart2_rxtx_init(UART_BAUD);

	while(1)
	{
		key = uart2_read();
		if (key == 1)
		{
			digitalWrite(USER_LED, HIGH);
		}
		else
		{
			digitalWrite(USER_LED, LOW);
		}
	}

	return 0;
}
