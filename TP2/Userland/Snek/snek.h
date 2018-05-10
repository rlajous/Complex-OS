#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define GAME_WIDTH 80
#define GAME_HEIGHT 25
#define TAIL_LENGTH_LIMIT 80

typedef struct Vector2{
	int x;
	int y;
} vector2_t;

typedef struct Tail{
	char sprite;
	int length;
	vector2_t tailPositions[TAIL_LENGTH_LIMIT];
	int oldestElemIndex;
	int youngestElemIndex;
} tail_t;

typedef struct Player{
	char sprite;
	vector2_t position;
	vector2_t speed;
} player_t;

typedef struct Snack{
	char sprite;
	vector2_t position;
} snack_t;


void handleInput(char* inputKey, player_t* player);
void update(player_t* player, tail_t* tail, snack_t* snack, int* playerAlive);
void updatePlayer(player_t* player, int* playerAlive);
void updateTail(player_t* player, tail_t* tail);
void checkCollisionWithSnack(player_t* player, tail_t* tail, snack_t* snack);
void checkCollisionWithTail(player_t* player, tail_t* tail, int* playerAlive);
void draw(player_t* player, tail_t* tail, snack_t* snack);
