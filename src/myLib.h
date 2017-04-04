/**
 * @file
 * @author Cem Gokmen
 * @date 2 Apr 2017
 * @brief This file contains generic library functions and includes the other headers.
 */

/** An unsigned 32-bit (4-byte) type */
typedef unsigned int u32;

/** An unsigned 16-bit (2-byte) type */
typedef unsigned short u16;

/** An unsigned 8-bit (1-byte) type. Note that this type cannot be written onto RAM directly. */
typedef unsigned char u8;

/** The video buffer pointer for drawing in Mode 3 */
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

/**
 * Runs a blocking loop.
 * @param n How long to run loop for
 */
void delay(int n);

/**
 * Runs a blocking loop that checks for key presses during its process.
 * @param n How long to run loop for
 * @return  A merged bitvector of all key presses
 */
u32 keySensitiveDelay(int n);

/**
 * Runs a blocking loop until the start of next VBlank.
 */
void waitForVBlank();

/**
 * Generates a pseudo-random number between min and max, inclusive.
 *
 * I totally snatched this function from tonc.
 *
 * @param  min bottom end of range.
 * @param  max top end of range.
 * @return     random number in the given range.
 */
int qran_range(int min, int max);

#include "gbaGraphics.h"
#include "cnakeSettings.h"
#include "cnakeLogic.h"
#include "cnakeGraphics.h"
