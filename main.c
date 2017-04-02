// Make a simple GBA state machine
// Name: Cem Gokmen
#include "myLib.h"
#include "splash.h"

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
    int highScore = 0;

	while(1) {
		waitForVBlank();
		switch(state) {
		case START:
			drawFullScreenImage(splash);
			state = START_NODRAW;
			break;
		case START_NODRAW:
            pressed = KEY_DOWN_NOW(BUTTON_A);
            if (pressed && !previouslyPressed) {
                state = GAME_INIT;
            }

			break;
        case GAME_INIT:
			fillScreen3(BACKGROUND_COLOR);

            g = createGame();
            drawWalls();

			state = GAME;
			break;
		case GAME:
            if (g.snake.dead) {
                delay(GAME_OVER_DURATION);
                state = GAMEOVER;
            } else {
                // Work on existing state
				drawSnake(g.snake, BACKGROUND_COLOR);
                processGame(&g);
                drawSnake(g.snake, SNAKE_COLOR);
				delay(GAME_FRAME_DELAY);
            }

			break;
        case GAMEOVER:
			fillScreen3(BLUE);
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
