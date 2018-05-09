#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25
#define TAIL_LENGTH_LIMIT 80

typedef struct Vector2{
	int x;
	int y;
} vector2_t;

typedef struct Tail{
	char sprite = '.';
	int length = 0;
	vector2_t tailPositions[TAIL_LENGTH_LIMIT];
	int oldestElemIndex = 0;
	int youngestElemIndex = 0;
} tail_t;

typedef struct Player{
	char sprite = 'X';
	vector2_t position;
	vector2_t speed;
} player_t;

typedef struct Snack{
	char sprite = 'o';
	vector2_t position;
} snack_t;