/**
 * @file
 * @author Cem Gokmen
 * @date 2 Apr 2017
 * @brief This file contains the cnake settings.
 */

/** Width of the SMALL board */
#define SMALL_BOARD_WIDTH 60

/** Height of the SMALL board */
#define SMALL_BOARD_HEIGHT 34

/** Draw scale of the SMALL board */
#define SMALL_DRAW_SCALE 4

/** Initial length of the snake on the SMALL board */
#define SMALL_INITIAL_LENGTH 20


/** Width of the LARGE board */
#define LARGE_BOARD_WIDTH 120

/** Height of the LARGE board */
#define LARGE_BOARD_HEIGHT 70

/** Draw scale of the LARGE board */
#define LARGE_DRAW_SCALE 2
/** Initial length of the snake on the LARGE board */
#define LARGE_INITIAL_LENGTH 30

/**
 * Width of the snake board.
 *
 * This number is fetched from the SMALL and LARGE macros according to
 * the map size choice of the player.
 *
 * It is very important that no matter the player's choice,
 * this number multiplied by DRAW_SCALE equals 240.
 */
extern u32 SNAKE_BOARD_WIDTH;

/**
 * Height of the snake board.
 *
 * This number is fetched from the SMALL and LARGE macros according to
 * the map size choice of the player.
 *
 * It is very important that no matter the player's choice,
 * this number multiplied by DRAW_SCALE is less than 160.
 *
 * The remaining space on the screen will be used for the scoreboard.
 */
extern u32 SNAKE_BOARD_HEIGHT;

/**
 * Scale factor for drawing the snake board on the screen.
 *
 * This number is fetched from the SMALL and LARGE macros according to
 * the map size choice of the player.
 *
 * It is important to set this number correctly so that the drawing fills
 * up the screen but does not overflow. All ingame coordinates are multiplied
 * by this number for drawing.
 */
extern u32 DRAW_SCALE;

/**
 * Initial length of the snake.
 *
 * This number is fetched from the SMALL and LARGE macros according to
 * the map size choice of the player.
 *
 * Shorter snake lengths make more sense for smaller boards.
 */
extern u32 SNAKE_INITIAL_LENGTH;

/** Starting height of the scoreboard */
#define SCORE_BOX_Y SNAKE_BOARD_HEIGHT * DRAW_SCALE

/** How long the gameover image stays, as a delay parameter */
#define GAME_OVER_DURATION 150

/** Delay between two game frames, as a keySensitiveDelay parameter */
#define GAME_FRAME_DELAY 50

/** The snake's starting head x coordinate on the board */
#define SNAKE_START_X 20

/** The snake's starting head y coordinate on the board */
#define SNAKE_START_Y 20

/** The snake's starting facing direction on the board */
#define SNAKE_INITIAL_FACING RIGHT

/** Maximum number of turns that can be recorded (too small can cause major issues) */
#define MAX_TURN_COUNT 50

/** Number of food cycles after creation before a food expires*/
#define FOOD_DURATION 200

/** How much length will be added to the snake when he eats */
#define FOOD_LENGTH_INCREMENT 5

/** How improbable food generation is: larger numbers will take longer time */
#define FOOD_GENERATION_PROBABILITY_ONE_IN 50

/** The length of the food vector: must be greater than or equal to the largest food number */
#define MAX_FOOD_COUNT 5

/** Number of foods given a score */
#define FOODCOUNT(score) ((score) < 5000 ? 1 : ((score) < 10000 ? 2 : 3))

/** The speed of the snake given a score */
#define SNAKESPEED(score) 1

/** The background color of the game */
#define BACKGROUND_COLOR BLACK

/** The wall color of the game */
#define WALL_COLOR RED

/** The color of the snake */
#define SNAKE_COLOR GREEN

/** The color of food */
#define FOOD_COLOR WHITE

/** The color of the score on the scoreboard */
#define SCORE_COLOR YELLOW

/** The color of the scoreboard background */
#define SCORE_BACKGROUND_COLOR GRAY

/** How long the splash screen stays, as a vblank count */
#define SPLASH_COUNTDOWN 2000

/** Debug mode: if toggled on, scoreboard shows tick count */
#define DEBUG_MODE 0

/**
 * This sets the global board w/h, drawscale and snake initial length
 * variables according to the mapSize parameter.
 *
 * This was a last minute addition with all four variables originally
 * being concieved as macros, that's why such a sloppy implementation
 * had to be used.
 *
 * @param mapSize Size of map: 0 for small, anything else for large.
 */
void setMapSize(u16 mapSize);
