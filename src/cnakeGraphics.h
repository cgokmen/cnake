/**
 * @file
 * @author Cem Gokmen
 * @date 2 Apr 2017
 * @brief This file contains cnake rendering functions.
 */

/**
 * Draws a line between two game points.
 *
 * This function does not require the two points to be in increasing order.
 *
 * @param buffer Pointer to the buffer to draw onto
 * @param x1     The x coordinate of the starting point of the line
 * @param y1     The y coordinate of the starting point of the line
 * @param x2     The x coordinate of the ending point of the line
 * @param y2     The y coordinate of the ending point of the line
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawGameLineBetween(u16 *buffer, int x1, int x2, int y1, int y2, u8 color);

/**
 * Draws one game point. This will be an even number of pixels on the screen.
 *
 * @param buffer Pointer to the buffer to draw onto
 * @param x      The x coordinate of the pixel on the game plane
 * @param y      The y coordinate of the pixel on the game plane
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawGameDot(u16 *buffer, int x, int y, u8 color);

/**
 * Draws all components of a Game.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param g      Pointer to the game we want to draw.
 */
void drawGame(u16 *buffer, Game *g);

/**
 * Draws the scoreboard section of the game. This section contains the
 * current score, the current paused status and a cnake version label.
 * In debug mode, instead of the label it contains the current cycle count.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param g      Pointer to the game whose scoreboard we want to draw.
 */
void drawScore(u16 *buffer, Game *g);

/**
 * Draws the walls of the Snake board.
 *
 * @param buffer Pointer to the buffer to draw onto.
 */
void drawWalls(u16 *buffer);

/**
 * Draws a snake.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param s      Pointer to the snake we want to draw.
 */
void drawSnake(u16 *buffer, Snake *s);

/**
 * Draws a food.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param f      Pointer to the food we want to draw.
 */
void drawFood(u16 *buffer, Food *f);
