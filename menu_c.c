#include <stdio.h>
#include <stdint.h>

#include "snake.h"
#include "sdlhelper.h"

int menu(struct properties* props) {
	
	printf("Hier ist die Menu-Funktion!\n");
	
	props->v = 250;
	props->l = 50;
	
	//addbmpsdl(props, "img/snake.bmp", 50, 50, 256, 256);
	
	return 1;
}
