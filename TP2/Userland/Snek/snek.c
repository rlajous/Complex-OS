#include "snek.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
	player_t player;
	player_t player2;

	tail_t tail;
	tail_t tail2;
	snack_t snack;

	int playerAlive = 1;
	char inputKey = '0';

	int playerAlive2 = 1;

	player.sprite = 'X';
	player.position.x = 12;
	player.position.y = 5;
	player.speed.x = 1;
	player.speed.y = 0;

	tail.sprite = '.';
	tail.length = 0;
	tail.youngestElemIndex = 0;
	tail.oldestElemIndex = 0;

	player2.sprite = '#';
	player2.position.x = 20;
	player2.position.y = 5;
	player2.speed.x = 1;
	player2.speed.y = 0;

	tail2.sprite = '.';
	tail2.length = 0;
	tail2.youngestElemIndex = 0;
	tail2.oldestElemIndex = 0;

	snack.sprite = 'o';
	snack.position.x = 10;
	snack.position.y = 10;

	echo(0);
	while (playerAlive && playerAlive2){
	  sleep(55);
	  handleInput(&inputKey, &player, &player2);
	  update(&player, &player2, &tail, &tail2, &snack, &playerAlive, &playerAlive2);
	  draw(&player, &tail, &player2, &tail2, &snack);
	}
	clear();
	printf(playerAlive?"Player 1 wins!\n":"Player 2 Wins!\n");
	echo(1);


	return 0;
}

void handleInput(char* inputKey, player_t* player, player_t* player2){
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
		case 'i':
			(*player2).speed.x = 0;
			(*player2).speed.y = -1;
			break;
		case 'k':
			(*player2).speed.x = 0;
			(*player2).speed.y = 1;
			break;
		case 'l':
			(*player2).speed.x = 1;
			(*player2).speed.y = 0;
			break;
		case 'j':
			(*player2).speed.x = -1;
			(*player2).speed.y = 0;
			break;
	}	
}

void update(player_t* player, player_t* player2, tail_t* tail, tail_t* tail2, snack_t* snack, int* playerAlive, int* playerAlive2){
	updateTail(player, tail);
	updateTail(player2, tail2);
	updatePlayer(player, playerAlive);
	updatePlayer(player2, playerAlive2);
	checkCollisionWithSnack(player, tail, snack);
	checkCollisionWithSnack(player2, tail2, snack);
	checkCollisionWithTail(player, tail, playerAlive);
	checkCollisionWithTail(player, tail2, playerAlive);
	checkCollisionWithTail(player2, tail2, playerAlive2);
	checkCollisionWithTail(player2, tail, playerAlive2);
}

void draw(player_t* player, tail_t* tail, player_t* player2, tail_t* tail2, snack_t* snack){
	clear();
	for (int i = (*tail).oldestElemIndex; i < (*tail).youngestElemIndex; i++){
		writeCharAtScreenPos((*tail).sprite, (*tail).tailPositions[i].x, (*tail).tailPositions[i].y);
	}
	for (int i = (*tail2).oldestElemIndex; i < (*tail2).youngestElemIndex; i++){
		writeCharAtScreenPos((*tail2).sprite, (*tail2).tailPositions[i].x, (*tail2).tailPositions[i].y);
	}
	writeCharAtScreenPos((*player).sprite, (*player).position.x, (*player).position.y);
	writeCharAtScreenPos((*player2).sprite, (*player2).position.x, (*player2).position.y);
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
		(*tail).youngestElemIndex += 2;
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

