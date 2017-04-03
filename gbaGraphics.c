#include "myLib.h"

u16 *videoBuffer = (u16 *)0x6000000;

void setPixel(u16 *buffer, int row, int col, u16 color) {
	buffer[OFFSET(row, col, 240)] = color;
}

void drawRect(u16 *buffer, int col, int row, int width, int height, volatile u16 color) {
	for(int r = 0; r<height; r++) {
		for(int c = 0; c < width; c++) {
			*(buffer + OFFSET(row+r, col+c, 240)) = color;
		}
	}
}

void drawRect3(u16 *buffer, int col, int row, int width, int height, volatile u16 color) {
	for(int r = 0; r<height; r++) {
		DMA[3].cnt = 0;
		DMA[3].src = &color;
		DMA[3].dst = buffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void drawFullWidthRectangle(u16 *buffer, int row, int height, u16 color) {
	DMA[3].cnt = 0;
	DMA[3].src = &color;
	DMA[3].dst = buffer + OFFSET(row, 0, 240);
	DMA[3].cnt = 240 * height | DMA_SOURCE_FIXED | DMA_ON;
}

void drawGameDot(u16 *buffer, int x, int y, u16 color) {
	drawRect(buffer, x * DRAW_SCALE, y * DRAW_SCALE, DRAW_SCALE, DRAW_SCALE, color);
}

void drawGameLineBetween(u16 *buffer, int x1, int y1, int x2, int y2, u16 color) {
	int minX = (x1 < x2) ? x1 : x2;
	int maxX = (x1 < x2) ? x2 : x1;
	int width = maxX - minX + 1;

	int minY = (y1 < y2) ? y1 : y2;
	int maxY = (y1 < y2) ? y2 : y1;
	int height = maxY - minY + 1;

	drawRect3(buffer, minX * DRAW_SCALE, minY * DRAW_SCALE, width * DRAW_SCALE, height * DRAW_SCALE, color);
}

void drawFullScreenImage(u16 *buffer, u16 *image) {
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

void drawImage3 (u16 *buffer, int row, int col, int width, int height, u16 *image) {
    for(int r = 0; r<height; r++) {
		DMA[3].src = image + OFFSET(row+r, col, width);
		DMA[3].dst = buffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

void drawChar(u16 *buffer, int col, int row, char ch, u16 color) {
	for(int r = 0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel(buffer, row+r, col+c, color);
			}
		}
	}
}

void drawString(u16 *buffer, int col, int row, char *str, u16 color) {
	while(*str) {
		drawChar(buffer, col, row, *str++, color);
		col += 6;
	}
}
