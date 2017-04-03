#define SNAKE_BOARD_WIDTH 120
#define SNAKE_BOARD_HEIGHT 70
#define DRAW_SCALE 2 // Make sure this number is even (bc no word access)

// These are used for random number generation
#define TWOPOWGEQWIDTH 128 // Smallest power of two greater than or equal to the Board Width
#define TWOPOWGEQHEIGHT 128 // Smallest power of two greater than or equal to the Board Height

#define SCORE_BOX_Y SNAKE_BOARD_HEIGHT * DRAW_SCALE

#define GAME_OVER_DURATION 100
#define GAME_FRAME_DELAY 50

#define SNAKE_START_X 20
#define SNAKE_START_Y 20
#define SNAKE_INITIAL_LENGTH 30
#define SNAKE_INITIAL_FACING RIGHT
#define MAX_TURN_COUNT 50

#define FOOD_DURATION 200
#define FOOD_LENGTH_INCREMENT 10
#define FOOD_GENERATION_PROBABILITY_ONE_IN 50
#define MAX_FOOD_COUNT 5
#define FOODCOUNT(score) ((score) < 5000 ? 1 : ((score) < 10000 ? 2 : 3))
#define SNAKESPEED(score) 1

#define BACKGROUND_COLOR BLACK
#define WALL_COLOR RED
#define SNAKE_COLOR GREEN
#define FOOD_COLOR WHITE
#define SCORE_COLOR YELLOW
#define SCORE_BACKGROUND_COLOR GRAY

#define DEBUG_MODE 0
