#include <stdio.h>
#include <stdint.h>

#include "snake.h"
#include "sdlhelper.h"

int menu(struct properties* props) {
	
	printf("Hier ist die Menu-Funktion!\n");
	
	addbmpsdl(props, "img/snake.bmp", 0, 0, 256, 256);
	
	SDL_Delay(4000);
	
	return 1;
}
