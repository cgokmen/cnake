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
	Point location;
	Direction previouslyFacing;
} Turn;

typedef struct
{
    Point head;
	u16 length;
	u16 growToLength;
    Direction facing;

    // I really couldn't bother with keeping more than 50 turns :)
    Turn turns[50];
    u16 numTurns;

    u16 dead;
} Snake;

typedef struct
{
    Point location;
    u32 deleteOnCycle;
    u16 value;
} Food;

typedef struct
{
    Snake snake;

    // Couldnt really bother with keeping more than 50 foods either
    Food foods[50];
    u16 numFoods;

    u32 score;

    u32 currentCycle;
} Game;

// Game functions
Game createGame();
void processGame(Game *g, u32 keysPressed);

// Snake functions
Snake createSnake();
void turnSnake(Snake *s, Direction facing);
int checkSelfCollision(Snake s);
int checkWallCollision(Snake s);

// Food functions
Food createRandomFood(Game *g);
int checkFoodCollision(Snake s, Food f);
void eatFood(Food *f, Game *g);

// Auxiliary functions
int isBetween(Point point, Point p1, Point p2);
int distBetween(Point p1, Point p2);
Direction getOpposite(Direction d);
