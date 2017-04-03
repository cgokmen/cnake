#define SNAKE_BOARD_WIDTH 120
#define SNAKE_BOARD_HEIGHT 70
#define DRAW_SCALE 2

// These are used for random number generation
#define TWOPOWGEQWIDTH 128 // Smallest power of two greater than or equal to the Board Width
#define TWOPOWGEQHEIGHT 128 // Smallest power of two greater than or equal to the Board Height

#define SCORE_BOX_Y SNAKE_BOARD_HEIGHT * DRAW_SCALE
#define SCORE_COLOR YELLOW

#define GAME_OVER_DURATION 50
#define GAME_FRAME_DELAY 50

#define SNAKE_START_X 20
#define SNAKE_START_Y 20
#define SNAKE_INITIAL_LENGTH 30
#define SNAKE_INITIAL_FACING RIGHT

#define FOOD_DURATION 200
#define FOOD_LENGTH_INCREMENT 5
#define FOODCOUNT(score) 1
#define SNAKESPEED(score) 1

#define BACKGROUND_COLOR BLACK
#define WALL_COLOR RED
#define SNAKE_COLOR GREEN
#define FOOD_COLOR WHITE
