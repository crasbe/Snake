#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdbool.h>

/****************************************
 * Declaration of structures
 ****************************************/
 
struct properties {
	// Window properties
	unsigned int x;
	unsigned int y;
	bool gui;
	
	unsigned int l;
	unsigned int v;
	unsigned int score;
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
