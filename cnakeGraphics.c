#include "myLib.h"
#include <stdio.h>

void drawGame(u16 *buffer, Game *g) {
	//fillScreen3(buffer, BACKGROUND_COLOR);
	drawWalls(buffer);
	drawScore(buffer, *g);
	drawSnake(buffer, g->snake);

	for (int i = 0; i < g->numFoods; i++) {
		drawFood(buffer, g->foods[i]);
	}
}

void drawScore(u16 *buffer, Game g) {
	char scoreText[50];
	sprintf(scoreText, "Score: %d", g.score);

	drawFullWidthRectangle(buffer, SCORE_BOX_Y, 160 - SCORE_BOX_Y, GRAY);
	drawString(buffer, 24, SCORE_BOX_Y + 6, scoreText, SCORE_COLOR);
	drawString(buffer, 170, SCORE_BOX_Y + 6, "cnake v1.0", SCORE_COLOR);
}

void drawWalls(u16 *buffer) {
    drawGameLineBetween(buffer, 0, 0, 0, SNAKE_BOARD_HEIGHT - 1, WALL_COLOR); // Left vertical
	drawGameLineBetween(buffer, SNAKE_BOARD_WIDTH - 1, 0, SNAKE_BOARD_WIDTH - 1, SNAKE_BOARD_HEIGHT - 1, WALL_COLOR); // Right vertical
	drawGameLineBetween(buffer, 0, 0, SNAKE_BOARD_WIDTH - 1, 0, WALL_COLOR); // Top horizontal
	drawGameLineBetween(buffer, 0, SNAKE_BOARD_HEIGHT - 1, SNAKE_BOARD_WIDTH - 1, SNAKE_BOARD_HEIGHT - 1, WALL_COLOR); // Bottom horizontal
}

void drawSnake(u16 *buffer, Snake s) {
	Point current = s.head;
	Direction facing = getOpposite(s.facing);

	u16 turnIdx = 0;
	Turn turn;

	Point lineFrom = current;

	u16 remainingLength = s.length;

	Point safeCurrent;

	while (remainingLength > 0) {
		// Check if this is a turn
		if (turnIdx < s.numTurns) {
			turn = s.turns[turnIdx];
			if (current.x == turn.location.x && current.y == turn.location.y) {
				facing = getOpposite(turn.previouslyFacing);
				drawGameLineBetween(buffer, lineFrom.x, lineFrom.y, current.x, current.y, SNAKE_COLOR);
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

	drawGameLineBetween(buffer, lineFrom.x, lineFrom.y, current.x, current.y, SNAKE_COLOR);
}

void drawFood(u16 *buffer, Food f) {
    drawGameDot(buffer, f.location.x, f.location.y, FOOD_COLOR);
}
