#include <stdio.h>
#include <stdint.h>

#include "snake.h"
#include "sdlhelper.h"

#define PATH "highscore_c.txt"
#define MAX 64
#define MAX_NAME 16
#define MAX_LINES 10

#define STATE_QUIT 0
#define STATE_NAME 1
#define STATE_RUN 2
#define STATE_MENU 3

#define BT_NONE 0
#define BT_MENU 1
#define BT_QUIT 2

struct scoreElement {
	char name[MAX_NAME];
	unsigned int score;
};

unsigned int drawhighscore(struct properties* props, struct scoreElement* scoreList);
unsigned int sc_drawallbuttons(struct properties* props, unsigned int highlight);
unsigned int sc_xy2button(struct properties* props, int x, int y);

int score(struct properties* props) {
	
	// Variables
	struct scoreElement scoreList[MAX_LINES+1]; // store 10 elements from the file and the new one
	char buffer[MAX];
	FILE* scorefile;

	unsigned int newname = 0;
	unsigned int button = BT_NONE;
	unsigned int state = STATE_RUN;
	SDL_Event event;
	
	// make sure the name string is really empty
	for(int i = 0; i < MAX_NAME; i++) {
		scoreList[MAX_LINES].name[i] = 0;
	}
	
	// open highscore file
	scorefile = fopen(PATH, "r+");
	if(scorefile == NULL) {
		printf("Cannot open file: %s", PATH);
		return -1;
	}
	
	// read from file
	for(unsigned int i = 0; i < MAX_LINES; i++) {
		if(fgets(buffer, MAX, scorefile) != NULL) {
			sscanf(buffer, "%s %u", scoreList[i].name, &(scoreList[i].score));
		} else {
			printf("Error: Reached EOF. highscore file too small!\n");
			return -1;
		}
		if(i == MAX_LINES-1) {
			if(fgetc(scorefile) != EOF) {
				printf("Error: highscore file too big. Ignoring following lines.\n");
			}
		}
	}
	
	// ask for name if score > 0
	if(props->score == 0) {
		state = STATE_RUN;
		drawhighscore(props, scoreList);
	} else {
		state = STATE_NAME;
		scoreList[MAX_LINES].score = props->score;
		drawrectsdl(props, 0xFFFFFFFF, 160, 190, 320, 100);
		drawframesdl(props, 0xFFFF0000, 16, 160, 190, 480, 290);
		writetextsdl(props, 0xFF000000, 192, 226, FONT, 32, "Name: ");
	}
	
	while(state == STATE_RUN || state == STATE_NAME) {
		// wait for an event. this saves CPU load
		if(SDL_WaitEvent(&event) != 1) {
			printf("SDL_WaitEvent Error: %s", SDL_GetError());
			return -1;
		}
		
		// SDL window housekeeping...
		if(event.type == SDL_WINDOWEVENT) {
			// After minimizing and maximizing again, the window has to be redrawn
			if(event.window.event == SDL_WINDOWEVENT_EXPOSED) {
				// redraw everything
			} else if(event.window.event == SDL_WINDOWEVENT_CLOSE) { // someone pressed the X-button
				state = STATE_QUIT;
				continue; // don't do anything else after this
			}
		}
		
		// react to Ctrl-C
		if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_c) {
				if((SDL_GetModState() & KMOD_CTRL) == KMOD_RCTRL ||
					(SDL_GetModState() & KMOD_CTRL) == KMOD_LCTRL) {
						state = STATE_QUIT;
						continue; // don't do anything else after this
				}
			} 
			if(state == STATE_NAME) {
				if(event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') {
					printf("Entered char: %c\n", event.key.keysym.sym);
					if(newname < MAX_NAME) {
						// add new character to name array and write new name to screen
						if(newname == 0) {
							scoreList[MAX_LINES].name[newname] = (char)event.key.keysym.sym;
						} else {
							sprintf(scoreList[MAX_LINES].name, "%s%c", scoreList[MAX_LINES].name, (char)event.key.keysym.sym);
						}
						writetextsdl(props, 0xFF000000, 265, 226, FONT, 32, scoreList[MAX_LINES].name);
						
						newname++;
					}
				} else if(event.key.keysym.sym == SDLK_BACKSPACE) {
					// draw white rectangle over the text
					drawrectsdl(props, 0xFFFFFFFF, 265, 226, 199, 34);
					// remove last element and write new text
					if(newname > 0) {
						newname--;
						scoreList[MAX_LINES+1].name[newname] = 0;
						writetextsdl(props, 0xFF000000, 265, 226, FONT, 32, scoreList[MAX_LINES+1].name);
					}
				} else if(event.key.keysym.sym == SDLK_RETURN || newname == MAX_NAME) {
					printf("Name: %s\n", scoreList[MAX_LINES].name);
					
					// sort the array (bubble sort like with only one iteration
					for(int i = MAX_LINES; i > 0; i--) {
						if(scoreList[i].score > scoreList[i-1].score) {
							printf("%d %s %d\n", i, scoreList[i].name, scoreList[i].score);
							// swap the names
							char tmpname[MAX_NAME];
							strcpy(tmpname, scoreList[i].name);
							strcpy(scoreList[i].name, scoreList[i-1].name);
							strcpy(scoreList[i-1].name, tmpname);
							
							// swap the scores
							unsigned int tmpscore = scoreList[i].score;
							scoreList[i].score = scoreList[i-1].score;
							scoreList[i-1].score = tmpscore;
							
							printf("%s %d\n", scoreList[i].name, scoreList[i].score);
						}
					}
					
					// write to highscore file
					rewind(scorefile);
					for(unsigned int i = 0; i < MAX_LINES; i++) {
						sprintf(buffer, "%s %u\n", scoreList[i].name, scoreList[i].score);
						fputs(buffer, scorefile);
					}
					
					drawhighscore(props, scoreList);
					
					state = STATE_RUN;
				}
			}
		}
		// Mouse actions
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			int x, y;
			if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				button = sc_xy2button(props, x, y);
				printf("Button: %d\n", button);
				sc_drawallbuttons(props, button); // highlight the button, but do nothing yet
			}
		}
		if(event.type == SDL_MOUSEBUTTONUP) {
			// when the mousebutton lifted do something
			switch(button) {
				case BT_QUIT: 	state = STATE_QUIT; break;
				case BT_MENU:	state = STATE_MENU; break;
			}
			sc_drawallbuttons(props, BT_NONE);
			continue;
		}		
	}

	fclose(scorefile);
	
	if(state == STATE_QUIT) {
		return -1;
	} else if(state == STATE_MENU) {
		return 0;
	}
	
	return -1;
}

unsigned int drawhighscore(struct properties* props, struct scoreElement* scoreList) {
	// white background
	fillallsdl(props, 0xFFFFFFFF);
	
	// draw a blue frame
	drawframesdl(props, 0xFF00FF00, 16, 0, 60, 640, 420);
	
	writetextsdl(props, 0xFF000000, 200, 0, FONT, 64, "HIGHSCORES");
	
	// buffer for formatted strings
	char score[6];
	char name[MAX_NAME+3];
	
	for(int i = 0; i < MAX_LINES; i++) {
		sprintf(name, "%02d. %s", i+1, (scoreList+i)->name);
		writetextsdl(props, 0xFF000000, 20, 76+32*i, FONT, 32, name);
		
		sprintf(score, "%06d", (scoreList+i)->score);
		writetextsdl(props, 0xFF000000, 530, 76+32*i, FONT, 32, score);
	}
	
	// draw the buttons
	sc_drawallbuttons(props, BT_NONE);
	
	return 0;
}

// for a clean(er) implementation of drawallbuttons and xy2button see menu_c.c
unsigned int sc_drawallbuttons(struct properties* props, unsigned int highlight) {
	drawbuttonsdl(props, 13, 425, 300, 50, 6, (highlight == BT_MENU ? 0xFFFF0000 : 0xFF00FF00), "Neues Spiel", FONT, 32);
	drawbuttonsdl(props, 326, 425, 300, 50, 6, (highlight == BT_QUIT ? 0xFFFF0000 : 0xFF00FF00), "Beenden", FONT, 32);

	return 0;
}

unsigned int sc_xy2button(struct properties* props, int x, int y) {
	if(x < 0 || y < 0) {
		return BT_NONE;
	}
	unsigned int ux = (unsigned int)x;
	unsigned int uy = (unsigned int)y;
	
	// if the coordinates are out of the button field, no button has been clicked on
	if(ux < 13 || ux > 626 || 
	   uy < 425 || uy > 475) {
		return BT_NONE;
	} else if(uy >= 425 && uy <= 475) {
		if(ux >= 13 && ux <= 313) {
			return BT_MENU;
		} else if(ux >= 326 && ux <= 626) {
			return BT_QUIT;
		} else {
			return BT_NONE;
		}
	} else {
		printf("Error: wrong button coordinates!\n");
	}
	return BT_NONE;
	
}
