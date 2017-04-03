void drawGameLineBetween(u16 *buffer, int x1, int x2, int y1, int y2, u8);
void drawGameDot(u16 *buffer, int x, int y, u8 color);
void drawGame(u16 *buffer, Game *g);
void drawScore(u16 *buffer, Game g);
void drawWalls(u16 *buffer);
void drawSnake(u16 *buffer, Snake s);
void drawFood(u16 *buffer, Food f);
