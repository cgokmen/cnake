#include "myLib.h"

u16 *videoBuffer = (u16 *)0x6000000;

/**
 * setPixel3(u16 *buffer, int x, int y, u16 color)
 * Draws a single pixel in GBA Mode 3.
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of pixel to draw (0 is left)
 * @param y      Y coordinate of pixel to draw (0 is top)
 * @param color  Color to draw the pixel in (u16 color value)
 */
void setPixel3(u16 *buffer, int x, int y, u16 color) {
    buffer[OFFSET(y, x, 240)] = color;
}

/**
 * setPixel4(u16* buffer, int col, int row, u8 color)
 * Draws a single pixel in GBA Mode 4.
 *
 * Note that because of the lack of single-byte edit capability
 * on the VRAM hardware, this function ends up having to read two-pixel
 * bundles and edit half of it. If you are setting an even number
 * of pixels to a single color, this is not the optimal solution.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of pixel to draw (0 is left)
 * @param row    Y coordinate of pixel to draw (0 is top)
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void setPixel4(u16* buffer, int col, int row, u8 color) {
    /* find the offset which is the regular offset divided by two */
    unsigned short offset = OFFSET(row, col, 240) >> 1;

    /* read the existing pixel which is there */
    unsigned short pixel = buffer[offset];

    /* if it's an odd column */
    if (col & 1) {
        /* put it in the left half of the short */
        buffer[offset] = (color << 8) | (pixel & 0x00ff);
    } else {
        /* it's even, put it in the left half */
        buffer[offset] = (pixel & 0xff00) | color;
    }
}

/**
 * drawRect4(u16 *buffer, int col, int row, int width, int height, volatile u8 color)
 * Draws a rectangle in GBA Mode 4 without using DMA (useful for smaller rectangles)
 *
 * This function relies on the fact that all x coordinates and widths for our
 * snake game are even numbers to do a more optimal calculation than normally
 * done with setPixel4. As a result it will not work with odd x coordinates and widths.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of top left corner of rectangle
 * @param row    Y coordinate of top left corner of rectangle
 * @param width  Width of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawRect4(u16 *buffer, int col, int row, int width, int height, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
	for(int r = 0; r<height; r++) {
		for(int c = 0; c < width; c++) {
            buffer[(OFFSET(row+r, col+c, 240) >> 1)] = clr;
		}
	}
}

/**
 * drawRect4DMA(u16 *buffer, int col, int row, int width, int height, volatile u8 color)
 * Draws a rectangle in GBA Mode 4 using DMA (useful for larger rectangles)
 *
 * This function relies on the fact that all x coordinates and widths for our
 * snake game are even numbers to do a more optimal calculation than normally
 * done with setPixel4. As a result it will not work with odd x coordinates and widths.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of top left corner of rectangle
 * @param row    Y coordinate of top left corner of rectangle
 * @param width  Width of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawRect4DMA(u16 *buffer, int col, int row, int width, int height, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
    for(int r = 0; r<height; r++) {
		DMA[3].cnt = 0;
		DMA[3].src = &clr;
		DMA[3].dst = buffer + (OFFSET(row+r, col, 240) >> 1);
		DMA[3].cnt = (width >> 1) | DMA_SOURCE_FIXED | DMA_ON;
	}
}

/**
 * drawFullWidthRectangle4(u16 *buffer, int row, int height, u8 color)
 * Draws a full-width rectangle in GBA Mode 4 using DMA
 *
 * This function benefits from the fact that rows of full-width rectangles
 * are contiguous addresses in memory. This allows for a single DMA execution.
 *
 * This function relies on the fact that the width of the screen is an
 * even number to do a more optimal calculation than normally done with setPixel4.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param row    Y coordinate of top left corner of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawFullWidthRectangle4(u16 *buffer, int row, int height, u8 color) {
    u16 clr = color + (((u16) color) << 8);
	DMA[3].cnt = 0;
	DMA[3].src = &clr;
	DMA[3].dst = buffer + (OFFSET(row, 0, 240) >> 1);
	DMA[3].cnt = 120 * height | DMA_SOURCE_FIXED | DMA_ON;
}

/**
 * drawFullScreenImage3(u16 *buffer, u16 *image)
 * Draws a full screen image in GBA Mode 3 using DMA
 *
 * This function benefits from the fact that full screen images are
 * are contiguous addresses in memory. This allows for a single DMA execution.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param image  Pointer to the image to draw
 */
void drawFullScreenImage3(u16 *buffer, u16 *image) {
	DMA[3].cnt = 0;
    DMA[3].src = image;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

/**
 * drawImageNonBlackPixels4(u16 *buffer, u16 *image)
 * Draws the non-black pixels of an image as white pixels onto the buffer in Mode 4.
 *
 * This essentially means assuming black pixels to be transparent. Its specific
 * use case is drawing the skull and bones image onto the game screen at snake death.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param image  Pointer to the image to draw
 */
void drawImageNonBlackPixels4(u16 *buffer, u16 *image) {
    // This method simply draws nonblack pixels as white onto the buffer.
    for(int y = 0; y < 160; y++) {
		for(int x = 0; x < 240; x++) {
            if (image[OFFSET(y, x, 240)]) {
                setPixel4(buffer, x, y, 6);
            }
		}
	}
}

/**
 * fillScreen3(u16 *buffer, volatile u16 color)
 * Paints the entire buffer in GBA Mode 3 using DMA
 *
 * This function benefits from the fact that the entire buffer consists of
 * contiguous addresses in memory. This allows for a single DMA execution.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param color  The color to paint the buffer in.
 */
void fillScreen3(u16 *buffer, volatile u16 color) {
	DMA[3].cnt = 0;
    DMA[3].src = &color;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_FIXED | DMA_ON;
}

/**
 * fillScreen4(u16 *buffer, volatile u8 color)
 * Paints the entire buffer in GBA Mode 3 using DMA
 *
 * This function benefits from the fact that the entire buffer consists of
 * contiguous addresses in memory. This allows for a single DMA execution.
 *
 * This function relies on the fact that the width of the screen is an
 * even number to do a more optimal calculation than normally done with setPixel4.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param color  The color to paint the buffer in.
 */
void fillScreen4(u16 *buffer, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
	DMA[3].cnt = 0;
    DMA[3].src = &clr;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*80 | DMA_SOURCE_FIXED | DMA_ON;
}

/**
 * drawImage3(u16 *buffer, int row, int col, int width, int height, u16 *image)
 * Draws an image in GBA Mode 3 using DMA
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param row    X coordinate of the top left corner of the image
 * @param col    Y coordinate of the top left corner of the image
 * @param width  Width of the image (the image array, scaling is not supported)
 * @param height Height of the image (the image array, scaling is not supported)
 * @param image  Pointer to the image to draw
 */
void drawImage3(u16 *buffer, int row, int col, int width, int height, u16 *image) {
    for(int r = 0; r<height; r++) {
		DMA[3].src = image + OFFSET(row+r, col, width);
		DMA[3].dst = buffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

/**
 * drawChar3(u16 *buffer, int col, int row, char ch, u16 color)
 * Draws a single 6x8 character onto the buffer.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of the top left corner of the character
 * @param row    Y coordinate of the top left corner of the character
 * @param ch     The ASCII character to draw
 * @param color  Color to draw the pixel in (u16 color value)
 */
void drawChar3(u16 *buffer, int col, int row, char ch, u16 color) {
	for(int r = 0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel3(buffer, col+c, row+r, color);
			}
		}
	}
}

/**
 * drawString3(u16 *buffer, int col, int row, char *str, u16 color)
 * Draws a string of characters onto the buffer.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of the top left corner of the string
 * @param row    Y coordinate of the top left corner of the string
 * @param ch     The string to draw
 * @param color  Color to draw the pixel in (u16 color value)
 */
void drawString3(u16 *buffer, int col, int row, char *str, u16 color) {
	while(*str) {
		drawChar3(buffer, col, row, *str++, color);
		col += 6;
	}
}

/**
 * drawChar4(u16 *buffer, int col, int row, char ch, u8 color)
 * Draws a single 6x8 character onto the buffer.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of the top left corner of the character
 * @param row    Y coordinate of the top left corner of the character
 * @param ch     The ASCII character to draw
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawChar4(u16 *buffer, int col, int row, char ch, u8 color) {
	for(int r = 0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel4(buffer, col+c, row+r, color);
			}
		}
	}
}

/**
 * drawString4(u16 *buffer, int col, int row, char *str, u8 color)
 * Draws a string of characters onto the buffer.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param col    X coordinate of the top left corner of the string
 * @param row    Y coordinate of the top left corner of the string
 * @param ch     The string to draw
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawString4(u16 *buffer, int col, int row, char *str, u8 color) {
	while(*str) {
		drawChar4(buffer, col, row, *str++, color);
		col += 6;
	}
}

/**
 * flipPage()
 * Changes the current video buffer and returns the address of the unused buffer.
 *
 * @return The address of the unused buffer for future drawing needs.
 */
u16* flipPage() {
  if(REG_DISPCNT & BUFFER1FLAG) {
    REG_DISPCNT &= ~BUFFER1FLAG;
    return BUFFER1;
  } else {
    REG_DISPCNT |= BUFFER1FLAG;
    return BUFFER0;
  }
}

/**
 * fillPalette()
 * Fills the color palette with the colors we need to draw.
 */
void fillPalette() {
  // We just need the background, snake, wall, food, score and scorebg colors.
  PALETTE[0] = BACKGROUND_COLOR;
  PALETTE[1] = WALL_COLOR;
  PALETTE[2] = SNAKE_COLOR;
  PALETTE[3] = FOOD_COLOR;
  PALETTE[4] = SCORE_COLOR;
  PALETTE[5] = SCORE_BACKGROUND_COLOR;
  PALETTE[6] = WHITE;
}
