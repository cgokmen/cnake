typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u16 *videoBuffer;

// Buttons
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & (key))
#define BUTTONS *(volatile u32 *)0x4000130

// Generic prototypes
void delay(int n);
u32 keySensitiveDelay(int n);
void waitForVBlank();
int qran_range(int min, int max);

#include "gbaGraphics.h"
#include "cnakeSettings.h"
#include "cnakeLogic.h"
#include "cnakeGraphics.h"
