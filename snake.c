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
	


	while(true) {
		
		SDL_Delay(16);
	}


	return 0;
}
