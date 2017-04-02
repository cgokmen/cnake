#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect3(int col, int row, int width, int height, unsigned short color)
{
	for(int r = 0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void drawGameDot(int x, int y, unsigned short color) {
	drawRect3(x * DRAW_SCALE, y * DRAW_SCALE, DRAW_SCALE, DRAW_SCALE, color);
}

void drawGameLineBetween(int x1, int y1, int x2, int y2, unsigned short color) {
	int minX = (x1 < x2) ? x1 : x2;
	int maxX = (x1 < x2) ? x2 : x1;
	int width = maxX - minX;

	int minY = (y1 < y2) ? y1 : y2;
	int maxY = (y1 < y2) ? y2 : y1;
	int height = maxY - minY;

	drawRect3(minX * DRAW_SCALE, minY * DRAW_SCALE, (width == 0 ? 1 : width) * DRAW_SCALE, (height == 0 ? 1 : height) * DRAW_SCALE, color);
}

void drawFullScreenImage(const u16* image) {
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void fillScreen3(unsigned short color) {
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 240*160 | DMA_SOURCE_FIXED | DMA_ON;
}

void delay(int n)
{
	int volatile x = 0;
	for(int i =0; i<n*8000; i++)
	{
		x = x + 1;
	}
}

void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER<159);
}

/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param row row to draw the image
* @param col column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3 (int row, int col, int width, int height, const u16*
image)
{
    for(int r = 0; r<height; r++)
	{
		DMA[3].src = image + OFFSET(row+r, col, width);
		DMA[3].dst = videoBuffer + OFFSET(row+r, col, 240);
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

// Snake Prototypes
Game createGame() {
    // Initialize the Snake
    Snake s = createSnake();

    // Initialize the Game
    Game g;
    g.snake = s;
    g.numFoods = 0;
    g.score = 0;

    return g;
}

void drawWalls() {
    drawGameLineBetween(0, 0, 0, SNAKE_BOARD_HEIGHT - 1, WALL_COLOR);
	drawGameLineBetween(SNAKE_BOARD_WIDTH - 1, 0, SNAKE_BOARD_WIDTH - 1, SNAKE_BOARD_HEIGHT - 1, WALL_COLOR);
	drawGameLineBetween(0, 0, SNAKE_BOARD_WIDTH - 1, 0, WALL_COLOR);
	drawGameLineBetween(0, SNAKE_BOARD_HEIGHT - 1, SNAKE_BOARD_WIDTH - 1, SNAKE_BOARD_HEIGHT - 1, WALL_COLOR);
}

Snake createSnake() {
    Snake s;
    Point head;
    head.x = SNAKE_START_X;
    head.y = SNAKE_START_Y;
    s.head = head;
    s.length = SNAKE_INITIAL_LENGTH;
    s.facing = SNAKE_INITIAL_FACING;
    s.numTurns = 0;
    s.dead = 0;

    return s;
}

void drawSnake(Snake s, unsigned short color) {
    // TODO: Implement
    //drawRect3(s.head.x * DRAW_SCALE, s.head.y * DRAW_SCALE, DRAW_SCALE, DRAW_SCALE, SNAKE_COLOR);

	Point current = s.head;
	Direction facing = getOpposite(s.facing);

	unsigned short turn = 0;
	Turn nextTurn = s.turns[turn];

	unsigned short remainingLength = s.length;

	while (remainingLength > 0) {
		// Paint this dot
		drawGameDot(current.x, current.y, color);

		// Check if this is a turn
		if (current.x == nextTurn.location.x && current.y == nextTurn.location.y) {
			facing = getOpposite(nextTurn.previouslyFacing);
			nextTurn = s.turns[++turn];
		}

		remainingLength--;

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

		// If we flew out, break.
		if (current.x <= 0 || current.x >= SNAKE_BOARD_WIDTH - 1 ||
			current.y <= 0 || current.y >= SNAKE_BOARD_HEIGHT - 1) {
				break;
		}
	}
}

Food createRandomFood(Game *g) {
    Food f;
	f.value = FOOD_SCORE;
	f.deleteOnCycle = g->currentCycle + FOOD_DURATION;

	Point p;
	p.x = 20;
	p.y = 20;
	f.location = p;

    // If it is at the location of
    // an existing food or the snake, recreate
    // random: rand()*N>>15

	drawFood(f);
    return f;
}

void drawFood(Food f) {
    drawGameDot(f.location.x, f.location.y, FOOD_COLOR);
}

void undrawFood(Food f) {
	drawGameDot(f.location.x, f.location.y, BACKGROUND_COLOR);
}

void processGame(Game *g) {
	// Start by incrementing game cycle
	g->currentCycle++;

    // Remove expired foods
    unsigned short newFoodCount = 0;
    for (unsigned short i = 1; i < g->numFoods; i++) {
        Food f = g->foods[i];
        if (g->currentCycle < f.deleteOnCycle) {
            g->foods[newFoodCount++] = f;
        } else {
            undrawFood(f);
        }
    }

    g->numFoods = newFoodCount;

    Snake s = g->snake;

    // Move the snake
    unsigned short speed = 1;

	switch (s.facing) {
	    case DOWN:
	        s.head.y += speed;
			break;
	    case RIGHT:
	        s.head.x += speed;
			break;
	    case UP:
	        s.head.y = s.head.y < speed ? speed : s.head.y;
	        s.head.y -= speed;
			break;
	    case LEFT:
	        s.head.x = s.head.x < speed ? speed : s.head.x;
	        s.head.x -= speed;
			break;
	   }

    // If either value is above MAX fix it
    s.head.x = s.head.x >= SNAKE_BOARD_WIDTH - 1 ? SNAKE_BOARD_WIDTH - 1 : s.head.x;
    s.head.y = s.head.y >= SNAKE_BOARD_HEIGHT - 1 ? SNAKE_BOARD_HEIGHT - 1 : s.head.y;

    // Now check if we have collided into anything
    s.dead = checkSelfCollision(s) || checkWallCollision(s);

    // Eat any food if necessary
    for (unsigned short i = 1; i < g->numFoods; i++) {
        Food f = g->foods[i];
        if (checkFoodCollision(s, f)) {
            eatFood(&f, g);
        }
    }

    // Generate any new foods if we must
	if (g->score == 0) {
	    while (g->numFoods < FOODCOUNT(g->score)) {
	        g->foods[g->numFoods++] = createRandomFood(g);
	    }
	}

    // Update snake facing
    if (KEY_DOWN_NOW(BUTTON_UP)) {
        turnSnake(&s, UP);
    } else if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
        turnSnake(&s, RIGHT);
    } else if (KEY_DOWN_NOW(BUTTON_DOWN)) {
        turnSnake(&s, DOWN);
    } else if (KEY_DOWN_NOW(BUTTON_LEFT)) {
        turnSnake(&s, LEFT);
    }

	g->snake = s;

    // And we're done!
}

int checkSelfCollision(Snake s) {
    // TODO: Implement
    return 0;
}

int checkWallCollision(Snake s) {
    return  s.head.x <= 0 ||
            s.head.x >= SNAKE_BOARD_WIDTH - 1 ||
            s.head.y <= 0 ||
            s.head.y >= SNAKE_BOARD_HEIGHT - 1;
}

int checkFoodCollision(Snake s, Food f) {
    return isBetween(f.location, s.head, s.turns[0].location);
}

void eatFood(Food *f, Game *g) {
    // We set the food to expire now so
    // that it will be deleted next cycle
    f->deleteOnCycle = g->currentCycle;
    g->score += f->value;

	Snake s = g->snake;
	s.length++;
	g->snake = s;
}

void turnSnake(Snake *s, Direction facing) {
    if (s->facing != facing) {
        if (getOpposite(s->facing) != facing) {
            // This is a valid turn! Move the turns by one.
            for (unsigned short i = 1; i < s->numTurns; i++) {
                s->turns[i] = s->turns[i - 1];
            }

			Turn t;
			t.location = s->head;
			t.previouslyFacing = s->facing;

            s->turns[0] = t;
            s->numTurns++;

			if (s->numTurns > 50) s->numTurns = 50;

            s->facing = facing;
        }
    }
}

int isBetween(Point point, Point p1, Point p2) {
    unsigned short minX = p1.x < p2.x ? p1.x : p2.x;
    unsigned short maxX = p1.x > p2.x ? p1.x : p2.x;

    unsigned short minY = p1.y < p2.y ? p1.y : p2.y;
    unsigned short maxY = p1.y > p2.y ? p1.y : p2.y;

    unsigned short x = point.x;
    unsigned short y = point.y;

    if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
        return 1;
    }

    return 0;
}

int distBetween(Point p1, Point p2) {
	if (p1.x != p2.x) {
		// This means the line is horizontal
		return (p1.x > p2.x) ? p1.x - p2.x : p2.x - p1.x;
	} else {
		return (p1.y > p2.y) ? p1.y - p2.y : p2.y - p1.y;
	}
}

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
