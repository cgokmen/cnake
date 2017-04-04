#include "myLib.h"
#include "splashImage.h"
#include "deadImage.h"
#include "logoImage.h"
#include <stdio.h>
#include <stdlib.h>

// State enum definition
typedef enum {
	SPLASH,
	SPLASH_NODRAW,
	START,
	START_NODRAW,
    GAME_INIT,
    GAME,
    GAMEOVER,
	GAMEOVER_NODRAW,
} GBAState;

int main() {
	GBAState state = SPLASH;

    u16 previouslyPressedA = 0;
	u16 pressedA = 0;

	u16 previouslyPressedSelect = 0;
	u16 pressedSelect = 0;

	u16 previouslyPressedStart = 0;
	u16 pressedStart = 0;

	u16 previouslyPressedLR = 0;
	u16 pressedLR = 0;

	short countDown = SPLASH_COUNTDOWN;
	u16 mapSize = 0;

    Game *g;
    u32 highScore = 0;
	u32 score = 0;

	u16 *currentBuffer;

	while(1) {
		pressedA = KEY_DOWN_NOW(BUTTON_A);
		pressedSelect = KEY_DOWN_NOW(BUTTON_SELECT);
		pressedStart = KEY_DOWN_NOW(BUTTON_START);
		pressedLR = KEY_DOWN_NOW(BUTTON_L | BUTTON_R);

		switch(state) {
		case SPLASH:
			waitForVBlank();
			drawFullScreenImage3(videoBuffer, (u16*) splashImage);
			REG_DISPCNT = MODE_3 | BG2_EN;

			state = SPLASH_NODRAW;
			break;
		case SPLASH_NODRAW:
            while (countDown > 0) {
				waitForVBlank();
				countDown--;
			}
			state = START;

			break;
		case START:
			waitForVBlank();
			fillScreen3(videoBuffer, BACKGROUND_COLOR);
			REG_DISPCNT = MODE_3 | BG2_EN;

			drawImage3(videoBuffer, 40, 5, 160, 40, (u16*) logoImage);
			drawCenteredString3(videoBuffer, 0, 50, 240, 10, "Current map size:", WHITE);

			if (!mapSize) {
				drawRect3DMA(videoBuffer, 70, 62, 40, 16, GREEN);
				drawRect3DMA(videoBuffer, 71, 63, 38, 14, BACKGROUND_COLOR);
			}
			drawCenteredString3(videoBuffer, 70, 62, 40, 16, "SMALL", YELLOW);

			if (mapSize) {
				drawRect3DMA(videoBuffer, 130, 62, 40, 16, GREEN);
				drawRect3DMA(videoBuffer, 131, 63, 38, 14, BACKGROUND_COLOR);
			}
			drawCenteredString3(videoBuffer, 130, 62, 40, 16, "LARGE", YELLOW);

			drawCenteredString3(videoBuffer, 0, 85, 240, 10, "Press L/R to change map size", WHITE);
			drawCenteredString3(videoBuffer, 0, 95, 240, 10, "Press A to start game", GREEN);

			drawCenteredString3(videoBuffer, 0, 110, 240, 10, "Controls:", WHITE);
			drawCenteredString3(videoBuffer, 0, 120, 240, 10, "Arrows to turn snake", WHITE);
			drawCenteredString3(videoBuffer, 0, 130, 240, 10, "START to play/pause", WHITE);
			drawCenteredString3(videoBuffer, 0, 140, 240, 10, "SELECT to reset game", WHITE);

			state = START_NODRAW;
			break;
		case START_NODRAW:
            if (pressedA && !previouslyPressedA) {
                state = GAME_INIT;
				break;
            }

			if (pressedLR && !previouslyPressedLR) {
				mapSize = !mapSize;

				waitForVBlank();
				drawRect3DMA(videoBuffer, 0, 62, 240, 16, BACKGROUND_COLOR);

				if (!mapSize) {
					drawRect3DMA(videoBuffer, 70, 62, 40, 16, GREEN);
					drawRect3DMA(videoBuffer, 71, 63, 38, 14, BACKGROUND_COLOR);
				}
				drawCenteredString3(videoBuffer, 70, 62, 40, 16, "SMALL", YELLOW);

				if (mapSize) {
					drawRect3DMA(videoBuffer, 130, 62, 40, 16, GREEN);
					drawRect3DMA(videoBuffer, 131, 63, 38, 14, BACKGROUND_COLOR);
				}
				drawCenteredString3(videoBuffer, 130, 62, 40, 16, "LARGE", YELLOW);
			}

			break;
        case GAME_INIT:
			setMapSize(mapSize);

			waitForVBlank();
			fillPalette();
			currentBuffer = flipPage();
			fillScreen4(currentBuffer, BACKGROUND_COLOR);
			REG_DISPCNT = MODE_4 | BG2_EN;
			currentBuffer = flipPage();

            g = createGame();

			state = GAME;
			break;
		case GAME:
            if (g->snake->dead) {
				// Draw the dead image
				drawGame(currentBuffer, g);
				drawImage4(currentBuffer, 0, 0, 240, (SNAKE_BOARD_HEIGHT - 1) * DRAW_SCALE, (u16*) deadImage);

				waitForVBlank();
				currentBuffer = flipPage();

                delay(GAME_OVER_DURATION);

				score = g->score;
				highScore = (score > highScore) ? score : highScore;
                state = GAMEOVER;

				// Let's get rid of the game.
				freeGame(g);
            } else {
				// Cancel the game if necessary
				if (pressedSelect && !previouslyPressedSelect) {
					freeGame(g);
					state = START;
					break;
				}

				// Pause / unpause the game if necessary
				if (pressedStart && !previouslyPressedStart) {
					togglePause(g);
				}

                // Process the state
                processGame(g, keySensitiveDelay(GAME_FRAME_DELAY));

				// We draw the game onto a temporary buffer first
				drawGame(currentBuffer, g);

				// We then wait for the VBlank and draw this buffer "image"
				// onto the screen.
				waitForVBlank();
				currentBuffer = flipPage();
            }

			break;
        case GAMEOVER:
			waitForVBlank();
			fillScreen3(videoBuffer, BLACK);
			REG_DISPCNT = MODE_3 | BG2_EN;

			char scoreText[100];
			char highScoreText[100];

			sprintf(scoreText, "Your score: %d", score);
			sprintf(highScoreText, "Your high score: %d", highScore);

			drawString3(videoBuffer, 10, 10, "Game Over :(", RED);
			drawString3(videoBuffer, 10, 30, scoreText, WHITE);
			drawString3(videoBuffer, 10, 40, highScoreText, WHITE);
			drawString3(videoBuffer, 10, 60, "Press A to retry", WHITE);

			drawString3(videoBuffer, 10, 80, "Credits:", GREEN);
			drawString3(videoBuffer, 10, 100, "cnake was developed for GT CS2110", WHITE);
			drawString3(videoBuffer, 10, 110, "by Cem Gokmen <cgokmen@gatech.edu>", WHITE);
			drawString3(videoBuffer, 10, 120, "(http://github.com/skyman/cnake)", WHITE);
			drawString3(videoBuffer, 10, 140, "2017, All Rights Reserved.", WHITE);

			state = GAMEOVER_NODRAW;
			break;
		case GAMEOVER_NODRAW:
            if ((pressedA && !previouslyPressedA) || (pressedSelect && !previouslyPressedSelect)) {
                state = START;
            }

			break;
		}

		previouslyPressedA = pressedA;
		previouslyPressedSelect = pressedSelect;
		previouslyPressedStart = pressedStart;
		previouslyPressedLR = pressedLR;
	}

	return 0;
}
