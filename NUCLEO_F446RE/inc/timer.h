/*
 * timer.h
 *
 *  Created on: Mar 22, 2023
 *      Author: kneaj
 */

#ifndef TIMER_H_
#define TIMER_H_

#define SYS_FREQ 16000000


void timerOCInit_2to5(uint8_t timer_num, uint16_t psc_val, uint16_t arr_val, uint8_t stm32_pin, uint32_t AFx, uint32_t CHx, uint32_t mode);
enum channels {CH1 = 1, CH2, CH3, CH4};


#endif /* TIMER_H_ */
