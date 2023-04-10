/*
 * i2c.h
 *
 *  Created on: Mar 26, 2023
 *      Author: kneaj
 */

#ifndef I2C_H_
#define I2C_H_


void i2c1_burstRead(uint8_t saddr, uint8_t maddr, int n, uint8_t *data);
void i2c1_burstWrite(uint8_t saddr, uint8_t maddr, int n, uint8_t *data);
void i2c1_byteRead(uint8_t saddr, uint8_t maddr, uint8_t *data);
void i2c1Init();


#endif /* I2C_H_ */
