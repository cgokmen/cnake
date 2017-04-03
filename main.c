#include "myLib.h"
#include "splashImage.h"
#include "deadImage.h"
#include <stdio.h>
#include <stdlib.h>

// State enum definition
enum GBAState {
	START,
	START_NODRAW,
    GAME_INIT,
    GAME,
    GAMEOVER,
	GAMEOVER_NODRAW,
};

int main() {
	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = START;

    short previouslyPressed = 0;
	short pressed = 0;

    Game g;
    u32 highScore = 0;
	u32 score = 0;

	u16 *currentBuffer;

	while(1) {
		switch(state) {
		case START:
			waitForVBlank();
			drawFullScreenImage3(videoBuffer, (u16*) splashImage);
			REG_DISPCNT = MODE_3 | BG2_EN;

			state = START_NODRAW;
			break;
		case START_NODRAW:
            pressed = KEY_DOWN_NOW(BUTTON_A);
            if (pressed && !previouslyPressed) {
                state = GAME_INIT;
            }

			break;
        case GAME_INIT:
			waitForVBlank();
			fillPalette();
			currentBuffer = flipPage();
			fillScreen4(currentBuffer, BACKGROUND_COLOR);
			REG_DISPCNT = MODE_4 | BG2_EN;

            g = createGame();

			state = GAME;
			break;
		case GAME:
            if (g.snake.dead) {
				// Draw the dead image
				drawGame(currentBuffer, &g);
				drawImageNonBlackPixels4(currentBuffer, (u16*) deadImage);

				waitForVBlank();
				currentBuffer = flipPage();

                delay(GAME_OVER_DURATION);

				score = g.score;
				highScore = (score > highScore) ? score : highScore;
                state = GAMEOVER;
            } else {
                // Process the state
                processGame(&g, keySensitiveDelay(GAME_FRAME_DELAY));

				// We draw the game onto a temporary buffer first
				drawGame(currentBuffer, &g);

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
            pressed = KEY_DOWN_NOW(BUTTON_A);
            if (pressed && !previouslyPressed) {
                state = START;
            }

			break;
		}

		previouslyPressed = pressed;
	}

	return 0;
}
