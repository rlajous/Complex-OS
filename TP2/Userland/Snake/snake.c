#include <snake.h>

int main(int argc, char *argv[]) {
	player_t player;
	tail_t tail;
	snack_t snack; 

	playerAlive = 1;
	char inputKey;
	snack.position.x = 10;
	snack.position.y = 10;

	while (playerAlive){
		//Input
		inputKey = getchar();
		switch(inputKey){
			case 'w':
				player.speed.x = 0;
				player.speed.y = 1;
				break;
			case 's':
				player.speed.x = 0;
				player.speed.y = -1;
				break;
			case 'd':
				player.speed.x = 1;
				player.speed.y = 0;
				break;
			case 'a':
				player.speed.x = -1;
				player.speed.y = 0;
		}
		//Update
		player.position.x += player.speed.y;
		player.position.x += player.speed.y;
		
		tail.tailPositions[tail.youngestElemIndex].x = player.position.x;
		tail.tailPositions[tail.youngestElemIndex].y = player.position.y;
		tail.youngestElemIndex++;
		tail.oldestElemIndex++;
		if (tail.youngestElemIndex >= TAIL_LENGTH_LIMIT)
			tail.youngestElemIndex = 0;
		if (tail.oldestElemIndex >= TAIL_LENGTH_LIMIT)
			tail.oldestElemIndex = 0;

		if (player.position.x = snack.position.x && player.position.y = snack.position.y){
			tail.length++;
			tail.oldestElemIndex--;
			snack.position.x = 15;
			snack.position.y = 12;
		}

		for (int i = tail.oldestElemIndex; i < tail.length; i++){
			if (tail.tailPositions[i].x == player.position.x && 
				tail.tailPositions[i].y == player.position.y)
				playerAlive = 0;
		}

		//Draw
		int80(2,0,0,0); //Clear Screen syscall
		drawCharAt(player.position.x, player.position.y, player.sprite);
		for (int i = tail.oldestElemIndex; i < tail.length; i++){
			drawCharAt(tail.tailPositions[i].x, tail.tailPositions[i].y, tail.sprite);
		}
		drawCharAt(snack.position.x, snack.position.y, snack.sprite);
	}

	return 0;
}


























