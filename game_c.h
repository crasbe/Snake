#ifndef GAME_C_H_
#define GAME_C_H_

#include "snake.h"

/*****************************************
 * Macros
 ****************************************/
// sizes
#define TILE 16
#define BORDERWIDTH TILE

// colors (ARGB)
#define BORDERCOL 0xFF990000
#define BACKGROUNDCOL 0xFF999999
#define SNAKECOL_NOSE 0xFF00BB00
#define SNAKECOL_BODY 0xFF009900
#define FOODCOL 0xFF000099

// game options
#define LEN_START 5 // Length of the snake at the beginning of the game

// controls
#define RIGHT 3
#define LEFT 4
#define UP 5
#define DOWN 6

// game states
#define STATE_RUN 0
#define STATE_PAUSE 1
#define STATE_QUIT 2

// return values
#define RET_SCORE 2
#define RET_QUIT -1

// font
#define FONT "fonts/clacon.ttf"

/*****************************************
 * Delaration of structures
 ****************************************/
// structure for the snake tiles
struct snakestruct {
	unsigned int x; // X-position of the tile
	unsigned int y; // X-position of the tile
	bool nose;		// true if this tile is the nose
	struct snakestruct* next; // the first element points to the last
};

// structure for the food tile
struct foodstruct {
	unsigned int x; // X-position of the food tile
	unsigned int y; // Y-position of the food tile
};

/*****************************************
 * Function prototypes
 ****************************************/
// little helpers
unsigned int interpretkey(SDL_Event* event);
uint32_t timerfunc(uint32_t interval, void *param);
unsigned int tile2pixel(unsigned int tilepos);
unsigned int writescore(struct properties* props);

// bigger helpers
unsigned int movesnake(struct properties* props, 
					   struct snakestruct** snakelist,
					   struct snakestruct** nose,
					   struct foodstruct* food,
					   unsigned int direction);
					   
unsigned int movesnaketopos(struct properties* props,
							struct snakestruct** snakelist,
							struct snakestruct** nose, bool food,
							unsigned int xpos, unsigned int ypos);

unsigned int drawplayfield(struct properties* props);

unsigned int drawsnake(struct properties* props,
					   struct snakestruct* snakelist);
					   
unsigned int placerandomfood(struct properties* props, 
							 struct foodstruct* food,
							 struct snakestruct* snakelist);

#endif // GAME_C_H_
