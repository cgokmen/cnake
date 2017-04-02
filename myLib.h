typedef unsigned int u32;
typedef unsigned short u16;

#define REG_DISPCNT *(unsigned short *)0x4000000
#define MODE_3 3
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
#define GRAY COLOR(25, 25, 25)

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

extern unsigned short *videoBuffer;

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
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130

// DMA
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
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

typedef struct
{
	unsigned int x;
    unsigned int y;
} Point;

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

typedef struct {
	Point location;
	Direction previouslyFacing;
} Turn;

typedef struct
{
    Point head;
	unsigned short length;
    Direction facing;

    // I really couldn't bother with keeping more than 50 turns :)
    Turn turns[50];
    unsigned short numTurns;

    unsigned short dead;
} Snake;

typedef struct
{
    Point location;
    unsigned int deleteOnCycle;
    unsigned short value;
} Food;

typedef struct
{
    Snake snake;

    // Couldnt really bother with keeping more than 50 foods either
    Food foods[50];
    unsigned short numFoods;

    unsigned int score;

    unsigned int currentCycle;
} Game;

// Snake stuff now
#define SNAKE_BOARD_WIDTH 120
#define SNAKE_BOARD_HEIGHT 70
#define DRAW_SCALE 2

#define GAME_OVER_DURATION 3
#define GAME_FRAME_DELAY 1

#define SNAKE_START_X 20
#define SNAKE_START_Y 20
#define SNAKE_INITIAL_LENGTH 10
#define SNAKE_INITIAL_FACING RIGHT

#define FOOD_DURATION 10000
#define FOOD_SCORE 10
#define FOODCOUNT(score) 1
#define SNAKESPEED(score) 1

#define BACKGROUND_COLOR BLACK
#define WALL_COLOR RED
#define SNAKE_COLOR GREEN
#define FOOD_COLOR WHITE

// Drawing Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect3(int row, int col, int width, int height, unsigned short);
void fillScreen3(unsigned short color);
void drawImage3 (int r, int c, int width, int height, const u16*
image);
void drawGameLineBetween(int x1, int x2, int y1, int y2, unsigned short);
void drawGameDot(int x, int y, unsigned short color);
void drawFullScreenImage(const u16* image);
void delay(int n);
void waitForVBlank();

// Snake Prototypes
Game createGame();
void drawWalls();

Snake createSnake();
void drawSnake(Snake s, unsigned short color);

Food createRandomFood(Game *g);
void drawFood(Food f);
void undrawFood(Food f);

void processGame(Game *g);
int checkSelfCollision(Snake s);
int checkWallCollision(Snake s);
int checkFoodCollision(Snake s, Food f);
void eatFood(Food *f, Game *g);
void turnSnake(Snake *s, Direction facing);

int isBetween(Point point, Point p1, Point p2);
int distBetween(Point p1, Point p2);
Direction getOpposite(Direction d);
