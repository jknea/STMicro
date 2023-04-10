/**
 ******************************************************************************
 * @file           : main.c
 * @author         : jknea
 * @brief          :
 ******************************************************************************
 *
 * The GPIOs (general purpose input/outputs) can sink or source up to +-8 mA, and sink or
 * source up to +-20 mA (with a relaxed VOL/VOH) except PC13, PC14 and PC15 which can
 * sink or source up to +-3mA.
 *
 *
 *
 *
 *
 ******************************************************************************
 */


#ifndef GPIO_H_
#define GPIO_H_


#define BIT0_MASK 	0b01
#define BIT1_MASK 	0b10

enum logics {LOW, HIGH};
enum modes {INPUT, OUTPUT, ALTERNATE, ANALOG};
enum gpio_pins {
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
	PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15,
	PH0, PH1, PH2, PH3, PH4, PH5, PH6, PH7, PH8, PH9, PH10, PH11, PH12, PH13, PH14, PH15
};

/* Arduino Pins as STM32 Pins */
#define D0		PA3
#define D1		PA2
#define D2		PA10
#define D3		PB3
#define D4		PB5
#define D5		PB4
#define D6		PB10
#define D7		PA8
#define D8		PA9
#define D9		PC7
#define D10		PB6
#define D11		PA7
#define D12		PA6
#define D13		PA5
#define D14		PB9
#define D15		PB8

#define A0		PA0
#define A1		PA1
#define A2		PA4
#define A3		PB0
#define A4		PC1
#define A5		PC0

#define USER_LED	PA5


/* Function Prototypes */
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin_name, uint8_t mode);


#endif /* GPIO_H_ */
