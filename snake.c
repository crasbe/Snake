#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "snake.h"
#include "sdlhelper.h"

int main(void) {
	// initialize struct
	struct properties* props;
	
	props = (struct properties*)malloc(sizeof(struct properties));
	
	props->x = 640;
	props->y = 480;
	
	if(initsdl(props) == 1) {
		printf("SDL initialization failed. Program quit.");
		free(props);
		return 1;
	}
	
	// the value of this variable defines what the program does next
	int nextop;
	nextop = menu(props);

	// see snake.h: -1 means "quit"
	while(nextop != -1) {
		if(nextop == 0)
			nextop = menu(props);
		else if(nextop == 1)
			nextop = game(props);
		else if(nextop == 2)
			nextop = score(props);
		else {
			printf("Illegal return value in main loop: %d!\n", nextop);
			break;
		}
	}

	cleanupsdl(props);
	free(props);
	return 0;
}
