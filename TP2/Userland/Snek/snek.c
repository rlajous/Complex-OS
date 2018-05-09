#include <snek.h>

int main(int argc, char *argv[]) {
	player_t player;
	tail_t tail;
	snack_t snack; 

	playerAlive = 1;
	char inputKey;
	snack.position.x = 10;
	snack.position.y = 10;

	while (playerAlive){
		handleInput(&inputKey, &player);
		update(&player, &tail, &snack);
		draw(&player, &tail, &snack);
	}

	return 0;
}

void handleInput(char* inputKey, player_t* player){
	*inputKey = getchar();
	switch(inputKey){
		case 'w':
			*player.speed.x = 0;
			*player.speed.y = 1;
			break;
		case 's':
			*player.speed.x = 0;
			*player.speed.y = -1;
			break;
		case 'd':
			*player.speed.x = 1;
			*player.speed.y = 0;
			break;
		case 'a':
			*player.speed.x = -1;
			*player.speed.y = 0;
	}	
}

void update(player_t* player, tail_t* tail, snack_t* snack){
	updatePlayer(player);
	updateTail(player, tail);
	checkCollisionWithSnack(player, snack);
	checkCollisionWithTail(player, tail);
}

void draw(player_t* player, tail_t* tail, snack_t* snack){
	int80(2,0,0,0); //Clear Screen syscall
	drawCharAt(player.position.x, player.position.y, player.sprite);
	for (int i = tail.oldestElemIndex; i < tail.length; i++){
		drawCharAt(tail.tailPositions[i].x, tail.tailPositions[i].y, tail.sprite);
	}
	drawCharAt(snack.position.x, snack.position.y, snack.sprite);
}

void updatePlayer(player_t* player){
	*player.position.x += *player.speed.y;
	*player.position.x += *player.speed.y;
}

void updateTail(player_t* player, tail_t* tail){
	*tail.tailPositions[*tail.youngestElemIndex].x = *player.position.x;
	*tail.tailPositions[*tail.youngestElemIndex].y = *player.position.y;
	*tail.youngestElemIndex++;
	*tail.oldestElemIndex++;
	if (*tail.youngestElemIndex >= TAIL_LENGTH_LIMIT)
		*tail.youngestElemIndex = 0;
	if (*tail.oldestElemIndex >= TAIL_LENGTH_LIMIT)
		*tail.oldestElemIndex = 0;
}

void checkCollisionWithSnack(player_t* player, snack_t* snack){
	if (*player.position.x = *snack.position.x && *player.position.y = *snack.position.y){
		*tail.length++;
		*tail.oldestElemIndex--;
		*snack.position.x = getSeconds() % GAME_WIDTH;
		*snack.position.y = getSeconds() % GAME_HEIGHT;
	}
}

void checkCollisionWithTail(player_t* player, tail_t* tail){
	for (int i = *tail.oldestElemIndex; i < *tail.length; i++){
		if (*tail.tailPositions[i].x == *player.position.x && 
			*tail.tailPositions[i].y == *player.position.y)
			playerAlive = 0;
	}
}

