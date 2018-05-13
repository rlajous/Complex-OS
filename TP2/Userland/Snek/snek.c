#include "snek.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
	player_t player;
	tail_t tail;
	snack_t snack;

	int playerAlive = 1;
	char inputKey = '0';

	player.sprite = 'X';
	player.position.x = 12;
	player.position.y = 5;
	player.speed.x = 1;
	player.speed.y = 0;

	tail.sprite = '.';
	tail.length = 0;
	tail.youngestElemIndex = 0;
	tail.oldestElemIndex = 0;

	snack.sprite = 'o';
	snack.position.x = 10;
	snack.position.y = 10;

	echo(0);
	while (playerAlive){
	  sleep(55);
	  handleInput(&inputKey, &player);
	  update(&player, &tail, &snack, &playerAlive);
	  draw(&player, &tail, &snack);
	}

	echo(1);
	clear();

	return 0;
}

void handleInput(char* inputKey, player_t* player){
	*inputKey = getch();
	switch(*inputKey){
		case 'w':
			(*player).speed.x = 0;
			(*player).speed.y = -1;
			break;
		case 's':
			(*player).speed.x = 0;
			(*player).speed.y = 1;
			break;
		case 'd':
			(*player).speed.x = 1;
			(*player).speed.y = 0;
			break;
		case 'a':
			(*player).speed.x = -1;
			(*player).speed.y = 0;
			break;
	}	
}

void update(player_t* player, tail_t* tail, snack_t* snack, int* playerAlive){
	updatePlayer(player, playerAlive);
	checkCollisionWithSnack(player, tail, snack);
	checkCollisionWithTail(player, tail, playerAlive);
	updateTail(player, tail);
}

void draw(player_t* player, tail_t* tail, snack_t* snack){
	clear();
	for (int i = (*tail).oldestElemIndex; i < (*tail).youngestElemIndex; i++){
		writeCharAtScreenPos((*tail).sprite, (*tail).tailPositions[i].x, (*tail).tailPositions[i].y);
	}
	writeCharAtScreenPos((*player).sprite, (*player).position.x, (*player).position.y);
	writeCharAtScreenPos((*snack).sprite, (*snack).position.x, (*snack).position.y);
}

void updatePlayer(player_t* player, int* playerAlive){
	(*player).position.x += (*player).speed.x;
	(*player).position.y += (*player).speed.y;
	if ((*player).position.x < 0 || (*player).position.x > GAME_WIDTH || 
		(*player).position.y < 0 || (*player).position.y > GAME_HEIGHT)
		*playerAlive = 0;
}

void updateTail(player_t* player, tail_t* tail){
	(*tail).tailPositions[(*tail).youngestElemIndex].x = (*player).position.x;
	(*tail).tailPositions[(*tail).youngestElemIndex].y = (*player).position.y;
	(*tail).youngestElemIndex++;
	(*tail).oldestElemIndex++;
	if ((*tail).youngestElemIndex >= TAIL_LENGTH_LIMIT)
		(*tail).youngestElemIndex = 0;
	if ((*tail).oldestElemIndex >= TAIL_LENGTH_LIMIT)
		(*tail).oldestElemIndex = 0;
}

void checkCollisionWithSnack(player_t* player, tail_t* tail, snack_t* snack){
	if ((*player).position.x == (*snack).position.x && (*player).position.y == (*snack).position.y){
		(*tail).length++;
		(*tail).youngestElemIndex++;
		(*snack).position.x = getSeconds() % GAME_WIDTH;
		(*snack).position.y = getSeconds() % GAME_HEIGHT;
	}
}

void checkCollisionWithTail(player_t* player, tail_t* tail, int* playerAlive){
	for (int i = (*tail).oldestElemIndex; i < (*tail).youngestElemIndex; i++){
		if ((*tail).tailPositions[i].x == (*player).position.x && 
			(*tail).tailPositions[i].y == (*player).position.y)
			*playerAlive = 0;
	}
}

