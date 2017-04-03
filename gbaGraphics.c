#include "myLib.h"

u16 *videoBuffer = (u16 *)0x6000000;

void setPixel3(u16 *buffer, int x, int y, u16 color) {
    buffer[OFFSET(y, x, 240)] = color;
}

void setPixel4(u16 *buffer, int x, int y, u8 color) {
    u16 *dst= &buffer[OFFSET(y, x, 240)/2];  // Division by 2 due to u8/u16 pointer mismatch!
    if(x&1)
        *dst= (*dst& 0xFF) | (color<<8);    // odd pixel
    else
        *dst= (*dst&~0xFF) |  color;        // even pixel
}

void drawRect4(u16 *buffer, int col, int row, int width, int height, volatile u8 color) {
	for(int r = 0; r<height; r++) {
		for(int c = 0; c < width; c++) {
			setPixel4(buffer, row + r, col + c, color);
		}
	}
}

void drawRect4DMA(u16 *buffer, int col, int row, int width, int height, volatile u8 color) {
    u16 clr = color + (((u16) color) << 8);
    for(int r = 0; r<height; r++) {
		DMA[3].cnt = 0;
		DMA[3].src = &clr;
		DMA[3].dst = buffer + OFFSET(row+r, col, 240) / 2;
		DMA[3].cnt = (width / 2) | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void drawFullWidthRectangle4(u16 *buffer, int row, int height, u8 color) {
    u16 clr = color + (((u16) color) << 8);
	DMA[3].cnt = 0;
	DMA[3].src = &clr;
	DMA[3].dst = buffer + OFFSET(row, 0, 240) / 2;
	DMA[3].cnt = 120 * height | DMA_SOURCE_FIXED | DMA_ON;
}

void drawFullScreenImage3(u16 *buffer, u16 *image) {
	DMA[3].cnt = 0;
    DMA[3].src = image;
    DMA[3].dst = buffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
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
    DMA[3].cnt = 240*160 / 2 | DMA_SOURCE_FIXED | DMA_ON;
}

void drawImage3(u16 *buffer, int row, int col, int width, int height, u16 *image) {
    for(int r = 0; r<height; r++) {
		DMA[3].src = image + OFFSET(row+r, col, width);
		DMA[3].dst = buffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
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
}
