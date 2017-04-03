#include "myLib.h"

void delay(int n) {
	int volatile x = 0;
	for(int i =0; i<n*8000; i++)
	{
		x = x + 1;
	}
}

u32 keySensitiveDelay(int n) {
	u32 keys = 0;
	int volatile x = 0;
	for(int i =0; i<n*30; i++)
	{
		x = x + 1;
		keys |= ~(BUTTONS);
	}

	return keys;
}

void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 159);
}
