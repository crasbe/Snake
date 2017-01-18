#include <stdio.h>
#include <stdint.h>

#include "snake.h"
#include "sdlhelper.h"

#define LEN_START 5 // Length of the snake at the beginning of the game

// structure of the snake
struct snakestruct {
	unsigned int x;
	unsigned int y;
	struct snakestruct* next;
}

// function prototypes

int game(struct properties* props) {
	
	// initialization phase
	
	// pointer to the nose of the snake
	struct snakestruct* nose;
	
	// allocate memory for the snakelist
	struct snakestruct* snakelist;
	snakelist = (struct snakestruct*)malloc(LEN_START*sizeof(struct snakestruct*));
	nose = snakelist; // point to first element
	
	// create a looped linear list
	for(int i = 0; i < LEN_START; i++) {
		snakelist[i].x = 20+i;				// TODO: insert right numbers
		snakelist[i].y = 15;
		if(i == LEN_START-1)
			snakelist[i].next = nose;
		else
			snakelist[i].next = snakelist[i+1];
	}
	
	if(createplayfield(props) != 0) {
		printf("Failed to create playfield. Quit program.\n");
		return -1;
	}
	
	
	printf("Hier ist die Game-Funktion!\n");
	
	return 2;
}


// helper functions
int createplayfield(struct properties* props) {
	fillallsdl(props, 0xFFFFFFFF); // white background

	return 0;
}
