#include "myLib.h"
#include <stdio.h>

/**
 * drawGameDot
 * Draws one game point. This will be an even number of pixels on the screen.
 *
 * @param buffer Pointer to the buffer to draw onto
 * @param x      The x coordinate of the pixel on the game plane
 * @param y      The y coordinate of the pixel on the game plane
 * @param color  Color to draw the pixel in (u8 index of color on the palette)
 */
void drawGameDot(u16 *buffer, int x, int y, u8 color) {
	drawRect4(buffer, x * DRAW_SCALE, y * DRAW_SCALE, DRAW_SCALE, DRAW_SCALE, color);
}

/**
 * drawGameLineBetween
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
void drawGameLineBetween(u16 *buffer, int x1, int y1, int x2, int y2, u8 color) {
	int minX = (x1 < x2) ? x1 : x2;
	int maxX = (x1 < x2) ? x2 : x1;
	int width = maxX - minX + 1;

	int minY = (y1 < y2) ? y1 : y2;
	int maxY = (y1 < y2) ? y2 : y1;
	int height = maxY - minY + 1;

	// If horizontal, use DMA. If not, don't.
	if (height == 1) drawRect4DMA(buffer, minX * DRAW_SCALE, minY * DRAW_SCALE, width * DRAW_SCALE, height * DRAW_SCALE, color);
	else drawRect4(buffer, minX * DRAW_SCALE, minY * DRAW_SCALE, width * DRAW_SCALE, height * DRAW_SCALE, color);
}

/**
 * drawGame
 * Draws all components of a Game.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param g      Pointer to the game we want to draw.
 */
void drawGame(u16 *buffer, Game *g) {
	// TODO: We need this to check for key input at various points.
	fillScreen4(buffer, 0);
	drawWalls(buffer);
	drawScore(buffer, g);
	drawSnake(buffer, g->snake);

	for (int i = 0; i < g->numFoods; i++) {
		drawFood(buffer, g->foods[i]);
	}
}

/**
 * drawScore
 * Draws the scoreboard section of the game. This section contains the
 * current score, the current paused status and a cnake version label.
 * In debug mode, instead of the label it contains the current cycle count.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param g      Pointer to the game whose scoreboard we want to draw.
 */
void drawScore(u16 *buffer, Game *g) {
	char scoreText[50];
	sprintf(scoreText, "Score: %d", g->score);

	drawFullWidthRectangle4(buffer, SCORE_BOX_Y, 160 - SCORE_BOX_Y, 5);
	drawString4(buffer, 10, SCORE_BOX_Y + 6, scoreText, 4);
	if (g->paused) drawString4(buffer, 100, SCORE_BOX_Y + 6, "PAUSED", 4);
	if (DEBUG_MODE) {
		char charBuffer[10];
		sprintf(charBuffer, "%d", g->currentCycle);
		drawString4(buffer, 170, SCORE_BOX_Y + 6, charBuffer, 4);
	} else drawString4(buffer, 170, SCORE_BOX_Y + 6, "cnake v1.0", 4);
}

/**
 * drawWalls
 * Draws the walls of the Snake board.
 *
 * @param buffer Pointer to the buffer to draw onto.
 */
void drawWalls(u16 *buffer) {
    drawGameLineBetween(buffer, 0, 0, 0, SNAKE_BOARD_HEIGHT - 1, 1); // Left vertical
	drawGameLineBetween(buffer, SNAKE_BOARD_WIDTH - 1, 0, SNAKE_BOARD_WIDTH - 1, SNAKE_BOARD_HEIGHT - 1, 1); // Right vertical
	drawGameLineBetween(buffer, 0, 0, SNAKE_BOARD_WIDTH - 1, 0, 1); // Top horizontal
	drawGameLineBetween(buffer, 0, SNAKE_BOARD_HEIGHT - 1, SNAKE_BOARD_WIDTH - 1, SNAKE_BOARD_HEIGHT - 1, 1); // Bottom horizontal
}

/**
 * drawSnake
 * Draws a snake.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param s      Pointer to the snake we want to draw.
 */
void drawSnake(u16 *buffer, Snake *s) {
	Point current = *s->head;
	Direction facing = getOpposite(s->facing);

	u16 turnIdx = 0;
	Turn *turn;

	Point lineFrom = current;

	u16 remainingLength = s->length;

	Point safeCurrent;

	while (remainingLength > 0) {
		// Check if this is a turn
		if (turnIdx < s->numTurns) {
			turn = s->turns[turnIdx];
			if (current.x == turn->location->x && current.y == turn->location->y) {
				facing = getOpposite(turn->previouslyFacing);
				drawGameLineBetween(buffer, lineFrom.x, lineFrom.y, current.x, current.y, 2);
				lineFrom = current;
				turnIdx++;
			}
		}

		// Move the current ptr
		safeCurrent = current;
		switch (facing) {
			case UP:
				current.y--;
				break;
			case DOWN:
				current.y++;
				break;
			case LEFT:
				current.x--;
				break;
			case RIGHT:
				current.x++;
				break;
		}

		remainingLength--;

		// If we flew out, break.
		if (current.x <= 0 || current.x >= SNAKE_BOARD_WIDTH - 1 ||
			current.y <= 0 || current.y >= SNAKE_BOARD_HEIGHT - 1) {
				current = safeCurrent;
				break;
		}
	}

	drawGameLineBetween(buffer, lineFrom.x, lineFrom.y, current.x, current.y, 2);
}

/**
 * drawFood
 * Draws a food.
 *
 * @param buffer Pointer to the buffer to draw onto.
 * @param f      Pointer to the food we want to draw.
 */
void drawFood(u16 *buffer, Food *f) {
    drawGameDot(buffer, f->location->x, f->location->y, 3);
}
