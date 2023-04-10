#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "bmx160.h"
#include "systick.h"

#define USER_LED	D13

//OC modes
#define TOGGLE	3

#define AFUNC1  1
#define AFUNC2  2

#define TIMER2	2
#define TIMER3	3

uint8_t data_rec[23] = {0};
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

float fax, fay, faz;

int main()
{
	uart2_txrx_init();

	bmx160Init();

	while(1)
	{
		bmx160_read_all(0x04, data_rec);
		mx = ((data_rec[1]<<8) | data_rec[0]);
		my = ((data_rec[3]<<8) | data_rec[2]);
		mz = ((data_rec[5]<<8) | data_rec[4]);

		gx = ((data_rec[9]<<8) | data_rec[8]);
		gy = ((data_rec[11]<<8) | data_rec[10]);
		gz = ((data_rec[13]<<8) | data_rec[12]);

		ax = ((data_rec[15]<<8) | data_rec[14]);
		ay = ((data_rec[17]<<8) | data_rec[16]);
		az = ((data_rec[19]<<8) | data_rec[18]);

		fax = ax * 0.000061035;
		fay = ay *0.000061035;
		faz = az *0.000061035;

		printf("ax = %d, ay = %d, az = %d\r\n", ax, ay, az);
		systickMillis(1000);
	}

	return 0;
}

