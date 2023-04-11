/*
 * uart.h
 *
 *  Created on: Mar 20, 2023
 *      Author: kneaj
 */

#ifndef UART_H_

#define UART_H_

#include "stm32f446xx.h"
#include <stdio.h>

void uart2_txrx_init();
char uart2_read();

#endif /* UART_H_ */

