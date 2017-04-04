#include "myLib.h"
#include <stdlib.h>

/**
 * createGame
 * Creates a functional game struct in heap memory and returns a pointer to it.
 *
 * @return Pointer to the created game.
 */
Game* createGame() {
    // Initialize the Snake
    Snake *s = createSnake();

    // Initialize the Game
    Game *g = malloc(sizeof(Game));
    g->snake = s;
    g->numFoods = 0;
    g->score = 0;
    g->paused = 0;

    g->foods = malloc(MAX_FOOD_COUNT * sizeof(Food*));

    return g;
}

/**
 * freeGame
 * Frees up a game from memory including its members.
 *
 * @param g Pointer to the game to free up.
 */
void freeGame(Game *g) {
    freeSnake(g->snake);

    for (int i = 0; i < g->numFoods; i++) {
        freeFood(g->foods[i]);
    }

    free(g->foods);
    free(g);
}

/**
 * processGame
 * Main tick loop of the game. Evaluates key presses, moves snake, runs
 * collision checks, updates score, etc.
 *
 * @param g           Pointer to the game to process.
 * @param keysPressed Merged key press vector from delay time to process.
 */
void processGame(Game *g, u32 keysPressed) {
    if (!g->paused) {
	   // Start by incrementing game cycle
    	g->currentCycle++;

        // Remove expired foods
        u16 newFoodCount = 0;
        for (u16 i = 0; i < g->numFoods; i++) {
            Food *f = g->foods[i];
            if (g->currentCycle < f->deleteOnCycle) {
                g->foods[newFoodCount++] = f;
            } else {
                freeFood(f);
            }
        }

        g->numFoods = newFoodCount;

        Snake *s = g->snake;

        // Move the snake
        u16 speed = SNAKESPEED(g->score);

    	switch (s->facing) {
    	    case DOWN:
    	        s->head->y += speed;
    			break;
    	    case RIGHT:
    	        s->head->x += speed;
    			break;
    	    case UP:
    	        s->head->y = s->head->y < speed ? speed : s->head->y;
    	        s->head->y -= speed;
    			break;
    	    case LEFT:
    	        s->head->x = s->head->x < speed ? speed : s->head->x;
    	        s->head->x -= speed;
    			break;
    	}

    	// If the snake needs to be grown, do that:
        if ((g->currentCycle & 1) == 0) {
        	s->length += speed;
        	if (s->length > s->growToLength) s->length = s->growToLength;
        }

        // If either value is above MAX fix it
        s->head->x = s->head->x >= SNAKE_BOARD_WIDTH - 1 ? SNAKE_BOARD_WIDTH - 1 : s->head->x;
        s->head->y = s->head->y >= SNAKE_BOARD_HEIGHT - 1 ? SNAKE_BOARD_HEIGHT - 1 : s->head->y;

        // Now check if we have collided into anything
        s->dead = checkSelfCollision(s) || checkWallCollision(s);

        // Eat any food if necessary
        for (u16 i = 0; i < g->numFoods; i++) {
            Food *f = g->foods[i];
            if (checkFoodCollision(s, f)) {
                eatFood(f, g);
            }
        }

        // Generate any new foods if we must
        unsigned short missingFoods = FOODCOUNT(g->score) - g->numFoods;
        while (missingFoods > 0) {
            if (qran_range(1, FOOD_GENERATION_PROBABILITY_ONE_IN) == 1) {
                g->foods[g->numFoods++] = createRandomFood(g);
            }
            missingFoods--;
        }

    	keysPressed |= ~(BUTTONS);

        // Update snake facing
        if (keysPressed & BUTTON_UP) {
            turnSnake(s, UP);
        } else if (keysPressed & BUTTON_RIGHT) {
            turnSnake(s, RIGHT);
        } else if (keysPressed & BUTTON_DOWN) {
            turnSnake(s, DOWN);
        } else if (keysPressed & BUTTON_LEFT) {
            turnSnake(s, LEFT);
        }

    	// Update score once every 64 cycles
    	if ((g->currentCycle & 63) == 0) {
    		g->score += 2 * s->length;
    	}
    }

    // And we're done!
}

/**
 * togglePause
 * Toggles the pause state of a game.
 *
 * @param g Pointer to the game to toggle the pause state of.
 */
void togglePause(Game *g) {
    g->paused = !g->paused;
}

/**
 * createSnake
 * Creates a functional snake struct in heap memory and returns a pointer to it.
 *
 * @return Pointer to the created snake.
 */
Snake* createSnake() {
    Snake *s = malloc(sizeof(Snake));
    Point *head = malloc(sizeof(Point));
    head->x = SNAKE_START_X;
    head->y = SNAKE_START_Y;
    s->head = head;
    s->length = SNAKE_INITIAL_LENGTH;
	s->growToLength = SNAKE_INITIAL_LENGTH;
    s->facing = SNAKE_INITIAL_FACING;
    s->numTurns = 0;
    s->dead = 0;

    s->turns = malloc(MAX_TURN_COUNT * sizeof(Turn*));

    return s;
}

/**
 * freeSnake
 * Frees up a snake from memory including its members.
 *
 * @param s Pointer to the snake to free up.
 */
void freeSnake(Snake *s) {
    free(s->head);

    for (int i = 0; i < s->numTurns; i++) {
        freeTurn(s->turns[i]);
    }

    free(s->turns);

    free(s);
}

/**
 * turnSnake
 * Turns a snake: adds a turn to the Snake's turns vector and
 * updates the direction it's facing.
 *
 * @param s      [description]
 * @param facing [description]
 */
void turnSnake(Snake *s, Direction facing) {
    if (s->facing != facing) {
        if (getOpposite(s->facing) != facing) {
            // This is a valid turn! Move the turns by one.
            if (s->numTurns == MAX_TURN_COUNT) {
                freeTurn(s->turns[s->numTurns - 1]);
            }

            for (u16 i = s->numTurns; i > 0; i--) {
                s->turns[i] = s->turns[i - 1];
            }

			Turn *t = malloc(sizeof(Turn));
			t->location = malloc(sizeof(Point));
            t->location->x = s->head->x;
            t->location->y = s->head->y;
			t->previouslyFacing = s->facing;

            s->turns[0] = t;
            s->numTurns++;

			if (s->numTurns > MAX_TURN_COUNT - 1) s->numTurns = MAX_TURN_COUNT - 1;

            s->facing = facing;
        }
    }
}

/**
 * freeTurn
 * Frees up a turn from memory including its members.
 *
 * @param t Pointer to the turn to free up.
 */
void freeTurn(Turn *t) {
    free(t->location);
    free(t);
}

/**
 * checkSelfCollision
 * Checks if the snake is colliding with itself.
 *
 * For the sake of performance this only checks if the part between
 * the snake's head and its last turn crosses any other part of the snake.
 *
 * @param  s Pointer to the snake to check for self collisions.
 * @return   1 if collision exists, 0 otherwise.
 */
int checkSelfCollision(Snake *s) {
	// Get the minX minY all that
	if (s->numTurns < 3) {
		return 0;
	}

	Point *head = s->head;
	Point *lastTurn = s->turns[0]->location;

	// Travel through the snake
	Point current = *s->turns[0]->location;
	Direction facing = getOpposite(s->facing);
	u16 turnIdx = 0;
	Turn *turn;
	int removedDist = distBetween(head, lastTurn);
	u16 remainingLength = (s->length > removedDist) ? s->length - removedDist : 0;

	while (remainingLength > 0) {
		// Check if this is a turn
		if (turnIdx < s->numTurns) {
			turn = s->turns[turnIdx];
			if (current.x == turn->location->x && current.y == turn->location->y) {
				facing = getOpposite(turn->previouslyFacing);
				turnIdx++;
			}
		}

		// Move the current ptr
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

		if (isBetween(&current, head, lastTurn)) {
			return 1;
		}

		// If we flew out, break.
		if (current.x <= 0 || current.x >= SNAKE_BOARD_WIDTH - 1 ||
			current.y <= 0 || current.y >= SNAKE_BOARD_HEIGHT - 1) {
				break;
		}
	}

    return 0;
}

/**
 * checkWallCollision
 * Checks if the snake is colliding with a wall.
 *
 * @param  s Pointer to the snake to check for wall collisions.
 * @return   1 if collision exists, 0 otherwise.
 */
int checkWallCollision(Snake *s) {
    return  s->head->x <= 0 ||
            s->head->x >= SNAKE_BOARD_WIDTH - 1 ||
            s->head->y <= 0 ||
            s->head->y >= SNAKE_BOARD_HEIGHT - 1;
}

/**
 * createRandomFood
 * Creates a functional food struct in heap memory and returns a pointer to it.
 * The food will be placed at a random point on the map that is not occupied
 * by the snake.
 *
 * @param g Pointer to the game to add this food to.
 * @return  Pointer to the created food.
 */
Food* createRandomFood(Game *g) {
    Food *f = malloc(sizeof(Food));
	f->value = FOOD_LENGTH_INCREMENT;
	f->deleteOnCycle = g->currentCycle + FOOD_DURATION;

    u16 foodInSnake;
	Point *p = malloc(sizeof(Point));
    Snake *s = g->snake;

    do {
        p->x = qran_range((SNAKE_BOARD_WIDTH * 9) / 10, SNAKE_BOARD_WIDTH / 10);
        p->y = qran_range((SNAKE_BOARD_HEIGHT * 9) / 10, SNAKE_BOARD_HEIGHT / 10);

        foodInSnake = 0;

        // Trace the snake to see if the food is in it
        Point current = *s->head;
    	Direction facing = getOpposite(s->facing);
        u16 remainingLength = s->length;

    	u16 turnIdx = 0;
    	Turn *turn;

    	while (remainingLength > 0) {
            // Check the spot
            if (current.x == p->x && current.y == p->y) {
                foodInSnake = 1;
                break;
            }

    		// Check if this is a turn
    		if (turnIdx < s->numTurns) {
    			turn = s->turns[turnIdx];
    			if (current.x == turn->location->x && current.y == turn->location->y) {
    				facing = getOpposite(turn->previouslyFacing);
    				turnIdx++;
    			}
    		}

    		// Move the current ptr
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
    				break;
    		}
    	}
    } while (foodInSnake);

	f->location = p;

    return f;
}

/**
 * freeFood
 * Frees up a food from memory including its members.
 *
 * @param f Pointer to the food to free up.
 */
void freeFood(Food *f) {
    free(f->location);
    free(f);
}

/**
 * checkFoodCollision
 * Checks if the snake is colliding with a given food.
 *
 * For the sake of performance this only checks if the food is between
 * the snake's head and its last turn.
 *
 * @param  s Pointer to the snake to check for collisions.
 * @param  f Pointer to the food to check if the snake is colliding with.
 * @return   1 if collision exists, 0 otherwise.
 */
int checkFoodCollision(Snake *s, Food *f) {
	if (s->numTurns > 0) {
		if (isBetween(f->location, s->head, s->turns[0]->location)) {
			if (distBetween(f->location, s->head) <= s->length) {
				return 1;
			}
		}
	}

	// Otherwise we just follow the snake towards its back facing direction
	// until we run out of length.
	Point current = *s->head;
	Direction facing = getOpposite(s->facing);
	u16 remainingLength = s->length;

	while (remainingLength > 0) {
		// Check if the food is here
		if (current.x == f->location->x && current.y == f->location->y) {
			return 1;
		}

		// Move the current ptr
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
				break;
		}
	}

	return 0;
}

/**
 * eatFood
 * Makes the snake eat a food: adds the length increase, the score, and
 * marks the food for deletion.
 *
 * @param f Pointer to the food to be eaten.
 * @param g Pointer to the game to add the benefits to.
 */
void eatFood(Food *f, Game *g) {
    // We set the food to expire now so
    // that it will be deleted next cycle
    g->score += (f->deleteOnCycle - g->currentCycle) * 5;

    f->deleteOnCycle = g->currentCycle;

	Snake *s = g->snake;
	s->growToLength += f->value;
}

/**
 * isBetween
 * Checks if a given point is on the line segment between two other points.
 *
 * For the sake of performance this contains optimizations that cause
 * this function to work only with vertical and horizontal lines.
 *
 * @param  point Pointer to the point we want to check if on a line segment.
 * @param  p1    Pointer to one endpoint of the line segment.
 * @param  p2    Pointer to other endpoint of the line segment.
 * @return       1 if the point is on the line, 0 otherwise.
 */
int isBetween(Point *point, Point *p1, Point *p2) {
    u16 minX = p1->x < p2->x ? p1->x : p2->x;
    u16 maxX = p1->x > p2->x ? p1->x : p2->x;

    u16 minY = p1->y < p2->y ? p1->y : p2->y;
    u16 maxY = p1->y > p2->y ? p1->y : p2->y;

    u16 x = point->x;
    u16 y = point->y;

    if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
        return 1;
    }

    return 0;
}

/**
 * distBetween
 *
 * Gets the distance between two lines.
 *
 * For the sake of performance this contains optimizations that cause
 * this function to work only with two points on the same vertical or
 * horizontal line.
 *
 * @param  p1 Pointer to the point to find the distance from.
 * @param  p2 Pointer to the point to find the distance to.
 * @return    The distance between the two points.
 */
int distBetween(Point *p1, Point *p2) {
	if (p1->x != p2->x) {
		// This means the line is horizontal
		return (p1->x > p2->x) ? p1->x - p2->x : p2->x - p1->x;
	} else {
		return (p1->y > p2->y) ? p1->y - p2->y : p2->y - p1->y;
	}
}

/**
 * getOpposite
 * Given a facing direction, returns the opposite direction.
 *
 * @param  d The direction whose opposite we want to find.
 * @return   The opposite direction.
 */
Direction getOpposite(Direction d) {
	switch (d) {
		case UP:
			return DOWN;
		case DOWN:
			return UP;
		case LEFT:
			return RIGHT;
		case RIGHT:
			return LEFT;
	}

	return UP;
}
