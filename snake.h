/****************************************
 * This header contains the main structure
 * and function prototypes for the modules.
 ****************************************/
#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdbool.h>

#include <SDL2/SDL.h>

/****************************************
 * Macros
 ***************************************/
#define FONT "fonts/clacon.ttf" // font to be used in the game

/****************************************
 * Declaration of structures
 ****************************************/
struct properties {
	// Window properties
	unsigned int x;		// X-size of the window
	unsigned int y;		// Y-size of the window
	SDL_Window *win;	// SDL-Window
	SDL_Renderer *ren;	// SDL-Renderer
	
	// Game properties
	unsigned int l;		// snake length to be reached to win
	unsigned int v;		// velocity of the snake. this defines t
	unsigned int score;	// reached score of the last game
};

/****************************************
 * Function prototypes
 * **************************************/
// The three main functions
// Return values:
//  * "-1": quit
//  *  "0": menu
//  *  "1": game
//  *  "2": highscore

extern int menu(struct properties* props);
extern int game(struct properties* props);
extern int score(struct properties* props);

#endif // SNAKE_H_
