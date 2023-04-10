#include "gpio.h"
#include "timer.h"
#include "uart.h"

#define USER_LED	D13

//OC modes
#define TOGGLE	3

#define AFUNC1  1
#define AFUNC2  2

#define TIMER2	2
#define TIMER3	3

int main()
{
//	uart2_txrx_init();
//	printf("Uart initialized\r\n");
//	printf("hello world\r\n");
	//	tim2_pa5_output_compare();


	//void timerOCInit_2to5(TIM_TypeDef *TIMx, uint16_t psc_val, uint16_t arr_val, uint8_t stm32_pin, uint32_t AFx, uint32_t CHx, uint32_t mode)
//	timerOCInit_2to5(TIMER2, 1599, 10000, USER_LED, AFUNC1, CH1, TOGGLE); // toggle PA5 every second due to Timer 2
	timerOCInit_2to5(TIMER2, 1599, 10000, USER_LED, AFUNC1, CH1, TOGGLE); // toggle PA5 every second due to Timer 2
	timerOCInit_2to5(TIMER3, 1599, 10000, D5, AFUNC2, CH1, TOGGLE); // toggle PA5 every second due to Timer 2


	while(1)
	{
	}

	return 0;
}

