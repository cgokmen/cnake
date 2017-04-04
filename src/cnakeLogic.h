/**
 * @file
 * @author Cem Gokmen
 * @date 2 Apr 2017
 * @brief This file contains the cnake logic functions.
 */

/**
 * The Point struct is used for keeping coordinates on the Snake board.
 */
typedef struct
{
	/** X coordinate of the point */
	u32 x;

	/** Y coordinate of the point */
    u32 y;
} Point;

/**
 * The Direction enum is used for keeping track of the direction the snake is travelling.
 */
typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

/**
 * The Turn struct is used for keeping track of the turns the snake makes.
 */
typedef struct {
	/** The location of the turn */
	Point *location;

	/** The direction the snake was facing before turning */
	Direction previouslyFacing;
} Turn;

/**
 * The Snake struct is used for keeping track of the snake in general.
 */
typedef struct
{
	/** The position of the head of the snake */
    Point *head;

	/** The length of the snake */
	u16 length;

	/**
	 * The length the snake must grow to. This will be equal to length
 	 * unless the snake just ate a food and needs to grow.
	 */
	u16 growToLength;

	/** The direction the snake is currently facing */
    Direction facing;

	/** Whether or not the snake is dead */
	u8 dead;

	/** Number of turns the snake has made */
    u16 numTurns;

	/** Array of turn pointers to the turns the snake has made */
	Turn **turns;
} Snake;

/**
 * The Food struct is used for keeping track of the foods on the map.
 */
typedef struct
{
	/** The location of the food on the board */
    Point *location;

	/** What game cycle the food must be deleted on (essentially expiry date) */
    u32 deleteOnCycle;

	/** How much the snake will grow after eating this food */
    u16 value;
} Food;

/**
 * The Game struct is used for keeping track of the entire game.
 */
typedef struct
{
	/** The snake present ingame */
    Snake *snake;

	/** The current score */
	u32 score;

	/** The current cycle count (i.e. tick count) */
    u32 currentCycle;

	/** Whether or not the game is paused */
	u8 paused;

	/** Number of foods on the map */
    u16 numFoods;

	/** Array of food pointers to the foods on the map */
	Food **foods;
} Game;

/**
 * Creates a functional game struct in heap memory and returns a pointer to it.
 *
 * @return Pointer to the created game.
 */
Game* createGame();

/**
 * Frees up a game from memory including its members.
 *
 * @param g Pointer to the game to free up.
 */
void freeGame(Game *g);

/**
 * Main tick loop of the game. Evaluates key presses, moves snake, runs
 * collision checks, updates score, etc.
 *
 * @param g           Pointer to the game to process.
 * @param keysPressed Merged key press vector from delay time to process.
 */
void processGame(Game *g, u32 keysPressed);

/**
 * Toggles the pause state of a game.
 *
 * @param g Pointer to the game to toggle the pause state of.
 */
void togglePause(Game *g);

/**
 * Creates a functional snake struct in heap memory and returns a pointer to it.
 *
 * @return Pointer to the created snake.
 */
Snake* createSnake();

/**
 * Frees up a snake from memory including its members.
 *
 * @param s Pointer to the snake to free up.
 */
void freeSnake(Snake *s);

/**
 * Turns a snake: adds a turn to the Snake's turns vector and
 * updates the direction it's facing.
 *
 * @param s      [description]
 * @param facing [description]
 */
void turnSnake(Snake *s, Direction facing);

/**
 * Frees up a turn from memory including its members.
 *
 * @param t Pointer to the turn to free up.
 */
void freeTurn(Turn *t);

/**
 * Checks if the snake is colliding with itself.
 *
 * For the sake of performance this only checks if the part between
 * the snake's head and its last turn crosses any other part of the snake.
 *
 * @param  s Pointer to the snake to check for self collisions.
 * @return   1 if collision exists, 0 otherwise.
 */
int checkSelfCollision(Snake *s);

/**
 * Checks if the snake is colliding with a wall.
 *
 * @param  s Pointer to the snake to check for wall collisions.
 * @return   1 if collision exists, 0 otherwise.
 */
int checkWallCollision(Snake *s);

/**
 * Creates a functional food struct in heap memory and returns a pointer to it.
 * The food will be placed at a random point on the map that is not occupied
 * by the snake.
 *
 * @param g Pointer to the game to add this food to.
 * @return  Pointer to the created food.
 */
Food* createRandomFood(Game *g);

/**
 * Frees up a food from memory including its members.
 *
 * @param f Pointer to the food to free up.
 */
void freeFood(Food *f);

/**
 * Checks if the snake is colliding with a given food.
 *
 * For the sake of performance this only checks if the food is between
 * the snake's head and its last turn.
 *
 * @param  s Pointer to the snake to check for collisions.
 * @param  f Pointer to the food to check if the snake is colliding with.
 * @return   1 if collision exists, 0 otherwise.
 */
int checkFoodCollision(Snake *s, Food *f);

/**
 * Makes the snake eat a food: adds the length increase, the score, and
 * marks the food for deletion.
 *
 * @param f Pointer to the food to be eaten.
 * @param g Pointer to the game to add the benefits to.
 */
void eatFood(Food *f, Game *g);

/**
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
int isBetween(Point *point, Point *p1, Point *p2);

/**
 * Gets the distance between two points.
 *
 * For the sake of performance this contains optimizations that cause
 * this function to work only with two points on the same vertical or
 * horizontal line.
 *
 * @param  p1 Pointer to the point to find the distance from.
 * @param  p2 Pointer to the point to find the distance to.
 * @return    The distance between the two points.
 */
int distBetween(Point *p1, Point *p2);

/**
 * Given a facing direction, returns the opposite direction.
 *
 * @param  d The direction whose opposite we want to find.
 * @return   The opposite direction.
 */
Direction getOpposite(Direction d);
