#include "gpio.h"
#include "systick.h"

#define USER_LED	PA5


int main()
{
//	initGPIO(); // TODO: enable all gpio

	pinMode(USER_LED, OUTPUT);

	while(1)
	{
		digitalWrite(USER_LED, HIGH);
		systickMillis(1000);
		digitalWrite(USER_LED, LOW);
		systickMillis(1000);
	}

	return 0;
}
