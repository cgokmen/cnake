typedef struct
{
	u32 x;
    u32 y;
} Point;

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

typedef struct {
	Point *location;
	Direction previouslyFacing;
} Turn;

typedef struct
{
    Point *head;
	u16 length;
	u16 growToLength;
    Direction facing;
	u8 dead;

    u16 numTurns;
	Turn **turns;
} Snake;

typedef struct
{
    Point *location;
    u32 deleteOnCycle;
    u16 value;
} Food;

typedef struct
{
    Snake *snake;
	u32 score;
    u32 currentCycle;
	u8 paused;

    u16 numFoods;
	Food **foods;
} Game;

// Game functions
Game* createGame();
void freeGame(Game *g);
void processGame(Game *g, u32 keysPressed);
void togglePause(Game *g);

// Snake functions
Snake* createSnake();
void freeSnake(Snake *s);
void turnSnake(Snake *s, Direction facing);
void freeTurn(Turn *t);
int checkSelfCollision(Snake *s);
int checkWallCollision(Snake *s);

// Food functions
Food* createRandomFood(Game *g);
void freeFood(Food *f);
int checkFoodCollision(Snake *s, Food *f);
void eatFood(Food *f, Game *g);

// Auxiliary functions
int isBetween(Point *point, Point *p1, Point *p2);
int distBetween(Point *p1, Point *p2);
Direction getOpposite(Direction d);
