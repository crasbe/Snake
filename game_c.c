#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "snake.h"
#include "sdlhelper.h"

#include "game_c.h"


int game(struct properties* props) {
	
	/*************************************
	 * Initialization
	 ************************************/
/** Variables */
	// SDL variables
 	SDL_Event event;
	SDL_TimerID movetimer;
	
	unsigned int direction = LEFT;	// default direction is LEFT
	unsigned int state = STATE_RUN; // state of execution
	int ret = RET_SCORE;	// what will be displayed after the game?
	
	struct snakestruct* snakelist;	// pointer to the whole snake list
	struct snakestruct* nose;	// pointer to the nose of the snake
	
	struct foodstruct* food;	// out little food tile
	
	
/** Memory allocation */
	// allocate for the maximum length, so no realloc has to be used
	snakelist = (struct snakestruct*)malloc(props->l*sizeof(struct snakestruct));
	food = (struct foodstruct*)malloc(sizeof(struct foodstruct));
	
	nose = &snakelist[0]; // point to first element
	
	// The score is used to calculate the current length of the snake
	// so it's initialized with a score.
	// This will be substracted later.
	props->score = (1001-props->v) * LEN_START;
	
	
/** Draw playfield */
	if(drawplayfield(props) != 0) {
		printf("Failed to draw playfield. Quit program.\n");
		return -1;
	}
	
	
/** Initialize and the snake */
	for(int i = 0; i < LEN_START; i++) {
		snakelist[i].x = (props->x/2)/TILE+i;	// place snake in the middle of the screen
		snakelist[i].y = (props->y/2)/TILE;
		
		snakelist[i].next = &snakelist[i-1];
		
		if(i == 0)
			snakelist[i].nose = true;
		else
			snakelist[i].nose = false;
		
		if(i == LEN_START-1) // last element
			snakelist[0].next = &snakelist[i];
	
		// draw the piece we just generated
		drawrectsdl(props, (snakelist[i].nose ? SNAKECOL_NOSE : SNAKECOL_BODY), 
					tile2pixel(snakelist[i].x), tile2pixel(snakelist[i].y), 
					TILE, TILE);
	}
	
	// place the first piece of food
	placerandomfood(props, food, snakelist);
	
	
/** Timer */
	movetimer = SDL_AddTimer(props->v, &timerfunc, NULL);
	if(movetimer == 0) {
		printf("SDL_AddTimer Error: %s", SDL_GetError());
		return -1;
	}
	
	
/** Main Loop */
	while(state != STATE_QUIT) {
		// wait for an event. this saves CPU load
		if(SDL_WaitEvent(&event) != 1) {
			printf("SDL_WaitEvent Error: %s", SDL_GetError());
			return -1;
		}
		
		// a key was pressed
		if(event.type == SDL_KEYDOWN) {
			unsigned int key = interpretkey(&event);
			if(key == STATE_QUIT) {
				ret = RET_QUIT;
				state = STATE_QUIT; // quit
			} else if(key == STATE_PAUSE) {
				// toggle pause with P
				if(state == STATE_RUN) {
					state = STATE_PAUSE;
					// write pause message
					writetextsdl(props, 0xFFFF0000, (props->x)/2-56, 0, FONT, 20, "****PAUSE*****");
				} else {
					state = STATE_RUN;
					// remove pause message
					drawrectsdl(props, BACKGROUNDCOL, (props->x)/2-56, 0, 120, 16);
				}
			} else if(key != STATE_RUN) { // (wildcard key/state)
				// set new direction
				direction = key;
				// move the snake directly when key was pressed
				if(state == STATE_RUN) {
					if(movesnake(props, &snakelist, &nose, food, direction) == 1) {
						state = STATE_QUIT;
					}
				}
			}
		}
		
		// the timer was #triggered and the game is running
		if(event.type == SDL_USEREVENT && state == STATE_RUN) {
			if(movesnake(props, &snakelist, &nose, food, direction) == 1) {
				state = STATE_QUIT; // GAME OVER
			}
		}
		
		// SDL window housekeeping...
		if(event.type == SDL_WINDOWEVENT) {
			// After minimizing and maximizing again, the window has to be redrawn
			if(event.window.event == SDL_WINDOWEVENT_EXPOSED) {
				drawplayfield(props);
				drawsnake(props, snakelist);
				// draw the food piece
				drawrectsdl(props, FOODCOL, tile2pixel(food->x),
							tile2pixel(food->y), TILE, TILE);
			} else if(event.window.event == SDL_WINDOWEVENT_CLOSE) { // someone pressed the X-button
				ret = RET_QUIT;
				state = STATE_QUIT;
			}
		}	
	}
	
	// remove the start bonus we used to calculate the length
	props->score = (props->score)-(1001-props->v) * LEN_START;
	
	
	// we don't need the timer anymore
	SDL_RemoveTimer(movetimer);
	
	// let the memories go
	free(snakelist);
	free(food);
	
	// now to something else
	return ret;
}


/*****************************************
 * Helper Functions
 ****************************************/
 
/***********************************************************************
 * tile2pixel:
 *   This function calculates a pixel coordinate from the tile coordinte.
 * Arguments:
 *   unsigned int tilepos - Tile position
 * Return: unsigned int
 *   Pixel coordinate
 **********************************************************************/
unsigned int tile2pixel(unsigned int tilepos) {
	return tilepos * TILE;
}

/***********************************************************************
 * timerfunc:
 *   This is just a little function to push a new event (USEREVENT)
 *   on the event list in order to trigger the main loop to move
 *   the snake.
 * Arguments:
 *   uint32_t interval - time interval from the last timer run
 *   void *param - user supplied parameter from SDL_AddTimer()
 * Return: uint32_t
 *   set the next timer interval
 **********************************************************************/
uint32_t timerfunc(uint32_t interval, void *param) {
	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	
	SDL_PushEvent(&event);
	
	return interval;
}

/***********************************************************************
 * writescore:
 *   This functions updates the score on the screen.
 * Arguments:
 *   struct properties* props - pointer to props for window information
 * Return: unsigned int
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int writescore(struct properties* props) {
	char highscore[6];
	
	// format the score, without start bonus
	sprintf(highscore, "%06d", (props->score) - (1001-props->v) * LEN_START);
	// clear the previous score
	if(drawrectsdl(props, BACKGROUNDCOL, (props->x)-48, 0, 100, 16) != 0) {
		return 1;
	}
	// write new score on the screen
	if(writetextsdl(props, 0, (props->x)-48, -1, FONT, 20, highscore) != 0) {
		return 1;
	}
	
	return 0;
}

/***********************************************************************
 * drawplayfield:
 *   This function (re)draws the playfield without the snake.
 * Arguments:
 *   struct properties* props - pointer to props for window information
 * Return: unsigned int
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int drawplayfield(struct properties* props) {
	if(fillallsdl(props, BACKGROUNDCOL) != 0)
		return 1;
	if(drawframesdl(props, BORDERCOL, BORDERWIDTH, 0, tile2pixel(1),
					props->x, props->y) != 0)
		return 1;
	if(writetextsdl(props, 0, 0, 0, FONT, 20, "Highscore:") != 0)
		return 1;
	if(writescore(props) != 0)
		return 1;
	
	return 0;
}

/***********************************************************************
 * drawsnake:
 *   This function initially draws the snake.
 * Arguments:
 *   struct properties* props - pointer to custom struct with window info
 *   struct snakestruct* snake - pointer to the whole snake
 * Return: unsigned int
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int drawsnake(struct properties* props, struct snakestruct* snakelist) {
	// pointer to the first element
	struct snakestruct* current = &snakelist[0];

	do {
		// if nose=true, draw with nose color,
		// otherwise with bodycolor	
		if(drawrectsdl(props, (current->nose ? SNAKECOL_NOSE : SNAKECOL_BODY), 
					   tile2pixel(current->x), tile2pixel(current->y), 
					   TILE, TILE) != 0) {
			return 1;
		}
		current = current->next; 	// go to next element
	} while(current != snakelist); 		// done when we reach the beginning

	return 0; // success
}

/***********************************************************************
 * placerandomfood:
 *   This functions places a piece of food randomly on the map.
 * Arguments:
 *   struct properties* props - pointer to custom properties array
 *   struct foodstruct* food - pointer to the food tile
 *   struct snakestruct* snakelist - pointer to the snake, used to check
 * 									 for collisions
 * Return: unsigned int
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int placerandomfood(struct properties* props, 
							 struct foodstruct* food,
							 struct snakestruct* snakelist) {
	struct snakestruct* current = &snakelist[0];
	
	// some more randomness required
	srand(time(NULL));
	
	while(true) {
		// generate random coordinates
		food->x = (rand() % ((props->x / TILE)-2))+1;
		food->y = (rand() % ((props->y / TILE)-3))+2; // leave some space for highscore
		
		// iterate over the snakelist to see if there are collisions
		do {
			// collision detection
			if((food->x == current->x) && (food->x == current->y)) {
				current = &snakelist[0]; // start all over again
				break;
			}
			
			current = current->next; // go to next element
		} while(current != &snakelist[0]); // done when we reach the beginning
		
		// successfully iterated over the list without collisions
		if(current == &snakelist[0])
			break;
	}
	
	return drawrectsdl( props, FOODCOL, tile2pixel(food->x),
						tile2pixel(food->y), TILE, TILE);
}

/***********************************************************************
 * interpretkey:
 *   This function helps to distinguish which key was pressed.
 * Arguments:
 *   SDL_Event* event - the event we read the KeyPress event from
 * Return: unsigned int
 *   0: STATE_RUN: wildcard
 *   1: STATE_PAUSE: pause the game
 *   2: STATE_QUIT: quit the game
 *   3: RIGHT: 	'->' or 'D'
 *   4: LEFT:	'<-' or 'A'
 *   5: UP:		 '^' or 'W'
 *   6: DOWN	 'V' or 'S'
 **********************************************************************/
 unsigned int interpretkey(SDL_Event* event) {	
	// Crtl-C
	if(event->key.keysym.sym == SDLK_c) {
		if((SDL_GetModState() & KMOD_CTRL) == KMOD_RCTRL ||
			(SDL_GetModState() & KMOD_CTRL) == KMOD_LCTRL)
			return STATE_QUIT;
	}
	else if(event->key.keysym.sym == SDLK_p) // Pause
		return STATE_PAUSE;
	else if(event->key.keysym.sym == SDLK_d ||  // D or RIGHT
			event->key.keysym.sym == SDLK_RIGHT)
		return RIGHT;
	else if(event->key.keysym.sym == SDLK_a ||  // A or LEFT
			event->key.keysym.sym == SDLK_LEFT)
	   return LEFT;
	else if(event->key.keysym.sym == SDLK_w ||  // W or UP
			event->key.keysym.sym == SDLK_UP)
		return UP;
	else if(event->key.keysym.sym == SDLK_s ||  // S or DOWN
	   		event->key.keysym.sym == SDLK_DOWN)
		return DOWN;
	
	return 5;
}

/***********************************************************************
 * movesnake:
 *   Move the snake one step further into a given direction.
 * Arguments:
 *   struct properties* props - pointer to custom property array
 *   struct snakestruct** snakelist - the whole snake
 *   struct snakestruct** nose - pointer to the nose of the snake
 *   struct foodstruct* food - pointer to the food tile
 *   unsigned int direction - direction for the snake to move
 * Return:
 *   0: move along
 *   1: collision/game over
 * Comment:
 *   The pointer to the pointer was necessary because we modify the
 *   dynamic array itself.
 **********************************************************************/

unsigned int movesnake(struct properties* props, 
					   struct snakestruct** snakelist, 
					   struct snakestruct** nose,
					   struct foodstruct* food, unsigned int direction) {
	unsigned int xpos = (*nose)->x;
	unsigned int ypos = (*nose)->y;
	unsigned int length = props->score / (1001-props->v);
	
	bool foodfield = false;
	
	switch(direction) {
		case LEFT:	xpos--;
					break;
		case RIGHT: xpos++;
					break;
		case UP:	ypos--;
					break;
		case DOWN:	ypos++;
					break;
		default:	printf("movesnake: Bad direction value!\n");
					return 1;
					break;
	}
	
	// did we hit a food field?
	if(food->x == xpos && food->y == ypos) {
		foodfield = true;
	}
	
	// change the current nose to body color
	if(drawrectsdl(props, SNAKECOL_BODY, tile2pixel((*nose)->x), 
				   tile2pixel((*nose)->y), TILE, TILE) != 0) {
		return 1;
	}
	
	if(foodfield == false) {
		// draw background color over the last element
		if(drawrectsdl(props, BACKGROUNDCOL, 
					   tile2pixel((*nose)->next->x), 
					   tile2pixel((*nose)->next->y), TILE, TILE) != 0) {
			return 1;
		}
		// the last element is now the first
		(*nose) = (*nose)->next;
		
	} else {
		// integrate the new element into the chain
		(*snakelist)[length].next = (*nose)->next;
		(*nose)->next = &(*snakelist)[length];
		
		// set nose flag
		(*nose)->nose = false;
		(*snakelist)[length].nose = true;
		
		// the new element is now the nose
		(*nose) = &(*snakelist)[length];
		
		// increase score
		props->score += 1001-props->v;
	}
	
	// set new coordinates to the 'last' or new element
	(*nose)->x = xpos;
	(*nose)->y = ypos;
	
	// draw the new nose
	if(drawrectsdl(props, SNAKECOL_NOSE, tile2pixel((*nose)->x), 
				   tile2pixel((*nose)->y), TILE, TILE) != 0) {
		return 1;
	}
	
	// Game over
	if(xpos == 0 || xpos == (props->x/TILE)-1 || 
	   ypos == 1 || ypos == (props->y/TILE)-1 || 
	   length >= props->l) {
		return 1;
	}
	
	// a new food item has to be placed
	if(foodfield == true) {
		placerandomfood(props, food, *snakelist);
		writescore(props); // update highscore
	}
	return 0;
}
