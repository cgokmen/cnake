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

void setPixel3(u16 *buffer, int row, int col, u16 color);
void setPixel4(u16 *buffer, int row, int col, u8 color);
void drawRect4(u16 *buffer, int row, int col, int width, int height, u8 color);
void drawRect4DMA(u16 *buffer, int row, int col, int width, int height, u8 color);
void drawFullWidthRectangle4(u16 *buffer, int row, int height, u8 color);
void drawFullScreenImage3(u16 *buffer, u16 *image);
void fillScreen3(u16 *buffer, volatile u16 color);
void fillScreen4(u16 *buffer, volatile u8 color);
void drawImage3 (u16 *buffer, int r, int c, int width, int height, u16 *image);
void drawChar3(u16 *buffer, int row, int col, char ch, u16 color);
void drawString3(u16 *buffer, int row, int col, char *str, u16 color);
void drawChar4(u16 *buffer, int row, int col, char ch, u8 color);
void drawString4(u16 *buffer, int row, int col, char *str, u8 color);
extern const unsigned char fontdata_6x8[12288];
u16* flipPage();
void fillPalette();
