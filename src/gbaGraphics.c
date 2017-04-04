#include "myLib.h"

u16 *videoBuffer = (u16 *)0x6000000;

void setPixel3(u16 *buffer, int x, int y, u16 color) {
    buffer[OFFSET(y, x, 240)] = color;
}

void drawRect3DMA(u16 *buffer, int col, int row, int width, int height, volatile u16 color) {
    for(int r = 0; r<height; r++) {
		DMA[3].cnt = 0;
		DMA[3].src = &color;
		DMA[3].dst = buffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

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

void drawRect4(u16 *buffer, int col, int row, int width, int height, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
	for(int r = 0; r<height; r++) {
		for(int c = 0; c < width; c++) {
            buffer[(OFFSET(row+r, col+c, 240) >> 1)] = clr;
		}
	}
}

void drawRect4DMA(u16 *buffer, int col, int row, int width, int height, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
    for(int r = 0; r<height; r++) {
		DMA[3].cnt = 0;
		DMA[3].src = &clr;
		DMA[3].dst = buffer + (OFFSET(row+r, col, 240) >> 1);
		DMA[3].cnt = (width >> 1) | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void drawFullWidthRectangle4(u16 *buffer, int row, int height, u8 color) {
    u16 clr = color + (((u16) color) << 8);
	DMA[3].cnt = 0;
	DMA[3].src = &clr;
	DMA[3].dst = buffer + (OFFSET(row, 0, 240) >> 1);
	DMA[3].cnt = 120 * height | DMA_SOURCE_FIXED | DMA_ON;
}

void drawFullScreenImage3(u16 *buffer, u16 *image) {
	DMA[3].cnt = 0;
    DMA[3].src = image;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

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

void fillScreen3(u16 *buffer, volatile u16 color) {
	DMA[3].cnt = 0;
    DMA[3].src = &color;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_FIXED | DMA_ON;
}

void fillScreen4(u16 *buffer, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
	DMA[3].cnt = 0;
    DMA[3].src = &clr;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*80 | DMA_SOURCE_FIXED | DMA_ON;
}

void drawImage3(u16 *buffer, int col, int row, int width, int height, u16 *image) {
    for(int r = 0; r<height; r++) {
		DMA[3].src = image + OFFSET(r, 0, width);
		DMA[3].dst = buffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

void drawImage4(u16 *buffer, int col, int row, int width, int height, u16 *image) {
    for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
            if (image[OFFSET(y, x, 240)]) {
                setPixel4(buffer, col+x, row+y, 6);
            } else {
                setPixel4(buffer, col+x, row+y, 0);
            }
		}
	}
}

void drawChar3(u16 *buffer, int col, int row, char ch, u16 color) {
	for(int r = 0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel3(buffer, col+c, row+r, color);
			}
		}
	}
}

void drawString3(u16 *buffer, int col, int row, char *str, u16 color) {
	while(*str) {
		drawChar3(buffer, col, row, *str++, color);
		col += 6;
	}
}

void drawCenteredString3(u16 *buffer, int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString3(buffer, col, row, str, color);
}

void drawChar4(u16 *buffer, int col, int row, char ch, u8 color) {
	for(int r = 0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel4(buffer, col+c, row+r, color);
			}
		}
	}
}

void drawString4(u16 *buffer, int col, int row, char *str, u8 color) {
	while(*str) {
		drawChar4(buffer, col, row, *str++, color);
		col += 6;
	}
}

void drawCenteredString4(u16 *buffer, int x, int y, int width, int height, char *str, u8 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString4(buffer, col, row, str, color);
}

u16* flipPage() {
  if(REG_DISPCNT & BUFFER1FLAG) {
    REG_DISPCNT &= ~BUFFER1FLAG;
    return BUFFER1;
  } else {
    REG_DISPCNT |= BUFFER1FLAG;
    return BUFFER0;
  }
}

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
