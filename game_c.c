#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "snake.h"
#include "sdlhelper.h"


#define TILE_X 16
#define TILE_Y 16

#define BORDERCOL 0xFF990000
#define BORDERWIDTH (TILE_X/2)

#define BACKGROUNDCOL 0xFF999999

#define LEN_START 5 // Length of the snake at the beginning of the game

#define SNAKECOL_NOSE 0xFF00BB00
#define SNAKECOL_BODY 0xFF009900

#define FOODCOL 0xFF000099

// structure of the snake
struct snakestruct {
	unsigned int x;
	unsigned int y;
	bool nose;
	struct snakestruct* next;
};

// structure for the food to be collected
struct foodstruct {
	unsigned int x;
	unsigned int y;
};

// function prototypes
unsigned int createplayfield(struct properties* props);
unsigned int drawsnakepiece(struct properties* props,  
							struct snakestruct* snakepiece);
unsigned int placerandomfood(struct properties* props, struct foodstruct* food);

int game(struct properties* props) {
	
	// initialization phase
	// --------------------
	
	// pointer to the nose of the snake
	struct snakestruct* nose;
	
	// allocate memory for the snakelist
	struct snakestruct* snakelist;
	snakelist = (struct snakestruct*)malloc(LEN_START*sizeof(struct snakestruct));
	nose = snakelist; // point to first element
	
	// draw playfield
	if(createplayfield(props) != 0) {
		printf("Failed to create playfield. Quit program.\n");
		return -1;
	}
	
	// create a looped linear list
	for(int i = 0; i < LEN_START; i++) {
		snakelist[i].x = (props->x/2)+(i*TILE_X);	// place snake in the middle of the screen
		snakelist[i].y = (props->y/2);
		if(i == LEN_START-1) {
			snakelist[i].next = nose;
			snakelist[i].nose = true;
			drawsnakepiece(props, &snakelist[i]);
		} else {
			snakelist[i].next = &snakelist[i+1];
			snakelist[i].nose = false;
			drawsnakepiece(props, &snakelist[i]);
		}
	}
	
	struct foodstruct* food = (struct foodstruct*)malloc(sizeof(struct foodstruct));
	
	placerandomfood(props, food);
	
	
	printf("Hier ist die Game-Funktion!\n");
	
	SDL_Delay(6000);
	
	return 2;
}


// helper functions
unsigned int createplayfield(struct properties* props) {
	fillallsdl(props, BACKGROUNDCOL);
	drawbordersdl(props, BORDERCOL, BORDERWIDTH);
	return 0;
}

unsigned int drawsnakepiece(struct properties* props, 
							struct snakestruct* snakepiece) {
	// if nose=true, draw with nose color,
	// otherwise with bodycolor	
	return drawrectsdl(	props, 
						(snakepiece->nose ? SNAKECOL_NOSE : SNAKECOL_BODY), 
						snakepiece->x, snakepiece->y, 
						TILE_X, TILE_Y);
}

unsigned int placerandomfood(struct properties* props, struct foodstruct* food) {
	srand(time(NULL));
	
	food->x = rand() % (props->x / TILE_X)+TILE_X;
	food->y = rand() % (props->y / TILE_Y)+TILE_Y;
	
	return drawrectsdl(props, FOODCOL, food->x, food->y, TILE_X, TILE_Y);
}
