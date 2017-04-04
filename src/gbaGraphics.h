/**
 * @file
 * @author Cem Gokmen
 * @date 2 Apr 2017
 * @brief This file contains GBA graphics functions.
 */

#define REG_DISPCNT *(u16 *)0x4000000
#define MODE_3 3
#define MODE_4 4
#define BG2_EN (1 << 10)
#define OFFSET(r, c, rowlen) ((c) + (r)*(rowlen))
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define GRAY COLOR(5, 5, 5)

#define BUFFER0 ((u16 *) 0x06000000)
#define BUFFER1 ((u16 *) 0x0600A000)
#define PALETTE ((u16 *) 0x05000000)
#define BUFFER1FLAG 0x10

#define SCANLINECOUNTER *(volatile u16 *)0x4000006

// DMA
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	u32 cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

/**
 * Draws a single pixel in GBA Mode 3.
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of pixel to draw (0 is left)
 * @param y      Y coordinate of pixel to draw (0 is top)
 * @param color  Color to draw the pixel in (u16 color value)
 */
void setPixel3(u16 *buffer, int x, int y, u16 color);

/**
 * Draws a rectangle in GBA Mode 3 using DMA.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of top left corner of rectangle
 * @param y      Y coordinate of top left corner of rectangle
 * @param width  Width of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u16 color value)
 */
void drawRect3DMA(u16 *buffer, int x, int y, int width, int height, u16 color);

/**
 * Draws a single pixel in GBA Mode 4.
 *
 * Note that because of the lack of single-byte edit capability
 * on the VRAM hardware, this function ends up having to read two-pixel
 * bundles and edit half of it. If you are setting an even number
 * of pixels to a single color, this is not the optimal solution.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of pixel to draw (0 is left)
 * @param y      Y coordinate of pixel to draw (0 is top)
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void setPixel4(u16 *buffer, int x, int y, u8 color);

/**
 * Draws a rectangle in GBA Mode 4 without using DMA (useful for smaller rectangles)
 *
 * This function relies on the fact that all x coordinates and widths for our
 * snake game are even numbers to do a more optimal calculation than normally
 * done with setPixel4. As a result it will not work with odd x coordinates and widths.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of top left corner of rectangle
 * @param y      Y coordinate of top left corner of rectangle
 * @param width  Width of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawRect4(u16 *buffer, int x, int y, int width, int height, u8 color);

/**
 * Draws a rectangle in GBA Mode 4 using DMA (useful for larger rectangles)
 *
 * This function relies on the fact that all x coordinates and widths for our
 * snake game are even numbers to do a more optimal calculation than normally
 * done with setPixel4. As a result it will not work with odd x coordinates and widths.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of top left corner of rectangle
 * @param y      Y coordinate of top left corner of rectangle
 * @param width  Width of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawRect4DMA(u16 *buffer, int x, int y, int width, int height, u8 color);

/**
 * Draws a full-width rectangle in GBA Mode 4 using DMA
 *
 * This function benefits from the fact that rows of full-width rectangles
 * are contiguous addresses in memory. This allows for a single DMA execution.
 *
 * This function relies on the fact that the width of the screen is an
 * even number to do a more optimal calculation than normally done with setPixel4.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param y      Y coordinate of top left corner of rectangle
 * @param height Height of rectangle
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawFullWidthRectangle4(u16 *buffer, int y, int height, u8 color);

/**
 * Draws a full screen image in GBA Mode 3 using DMA
 *
 * This function benefits from the fact that full screen images are
 * are contiguous addresses in memory. This allows for a single DMA execution.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param image  Pointer to the image to draw
 */
void drawFullScreenImage3(u16 *buffer, u16 *image);

/**
 * Draws the non-black pixels of an image as white pixels onto the buffer in Mode 4.
 *
 * This essentially means assuming black pixels to be transparent. Its specific
 * use case is drawing the skull and bones image onto the game screen at snake death.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param image  Pointer to the image to draw
 */
void drawImageNonBlackPixels4(u16 *buffer, u16 *image);

/**
 * Paints the entire buffer in GBA Mode 3 using DMA
 *
 * This function benefits from the fact that the entire buffer consists of
 * contiguous addresses in memory. This allows for a single DMA execution.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param color  The color to paint the buffer in.
 */
void fillScreen3(u16 *buffer, volatile u16 color);

/**
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
void fillScreen4(u16 *buffer, volatile u8 color);

/**
 * Draws an image in GBA Mode 3 using DMA
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of the top left corner of the image
 * @param y      Y coordinate of the top left corner of the image
 * @param width  Width of the image (the image array, scaling is not supported)
 * @param height Height of the image (the image array, scaling is not supported)
 * @param image  Pointer to the image to draw
 */
void drawImage3 (u16 *buffer, int x, int y, int width, int height, u16 *image);

/**
 * Draws an image in GBA Mode 4 using DMA.
 *
 * The image is drawn in Black and White (non-black pixels are white) in order
 * to enhance performance.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of the top left corner of the image
 * @param y      Y coordinate of the top left corner of the image
 * @param width  Width of the image (the image array, scaling is not supported)
 * @param height Height of the image (the image array, scaling is not supported)
 * @param image  Pointer to the image to draw
 */
void drawImage4 (u16 *buffer, int x, int y, int width, int height, u16 *image);

/**
 * Draws a single 6x8 character onto the buffer in Mode 3.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of the top left corner of the character
 * @param y      Y coordinate of the top left corner of the character
 * @param ch     The ASCII character to draw
 * @param color  Color to draw the text in (u16 color value)
 */
void drawChar3(u16 *buffer, int x, int y, char ch, u16 color);

/**
 * Draws a string of characters onto the buffer in Mode 3.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of the top left corner of the string
 * @param y      Y coordinate of the top left corner of the string
 * @param str    The string to draw
 * @param color  Color to draw the text in (u16 color value)
 */
void drawString3(u16 *buffer, int x, int y, char *str, u16 color);

/**
 * Draws a string of characters onto the buffer in Mode 3. The string will be
 * centered in the given bounding box.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of bounding box
 * @param y      Y coordinate of bounding box
 * @param width  Width of bounding box
 * @param height Height of bounding box
 * @param str    The string to draw
 * @param color  Color to draw the text in (u16 color value)
 */
void drawCenteredString3(u16 *buffer, int x, int y, int width, int height, char *str, u16 color);

/**
 * Draws a single 6x8 character onto the buffer in Mode 4.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of the top left corner of the character
 * @param y      Y coordinate of the top left corner of the character
 * @param ch     The ASCII character to draw
 * @param color  Color to draw the text in (u8 index of color on the palette)
 */
void drawChar4(u16 *buffer, int x, int y, char ch, u8 color);

/**
 * Draws a string of characters onto the buffer in Mode 4.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of the top left corner of the string
 * @param y      Y coordinate of the top left corner of the string
 * @param str    The string to draw
 * @param color  Color to draw the text in (u8 index of color on the palette)
 */
void drawString4(u16 *buffer, int x, int y, char *str, u8 color);

/**
 * Draws a string of characters onto the buffer in Mode 4. The string will be
 * centered in the given bounding box.
 *
 * @param buffer Pointer to the video buffer to draw onto
 * @param x      X coordinate of bounding box
 * @param y      Y coordinate of bounding box
 * @param width  Width of bounding box
 * @param height Height of bounding box
 * @param str    The string to draw
 * @param color  Color to draw the text in (u8 index of color on the palette)
 */
void drawCenteredString4(u16 *buffer, int x, int y, int width, int height, char *str, u8 color);

/**
 * Changes the current video buffer and returns the address of the unused buffer.
 *
 * @return The address of the unused buffer for future drawing needs.
 */
u16* flipPage();

/**
 * Fills the color palette with the colors we need to draw.
 */
void fillPalette();

/** Contains the pixels of each character from a 6x8 font */
extern const unsigned char fontdata_6x8[12288];
