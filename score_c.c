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

struct scoreElement {
	char name[MAX_NAME];
	unsigned int score;
};

int score(struct properties* props) {
	
	// Variables
	struct scoreElement scoreList[MAX_LINES+1]; // store 10 elements from the file and the new one
	char buffer[MAX];
	FILE* scorefile;
	int newelement = -1; // position of the new element
	unsigned int newname = 0;
	unsigned int state = STATE_RUN;
	SDL_Event event;
	
	// make sure the name string is really empty
	for(int i = 0; i < MAX_NAME; i++) {
		scoreList[MAX_LINES+1].name[i] = 0;
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
			sscanf(buffer, "%s %u", &(scoreList[i].name), &(scoreList[i].score));
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
	} else {
		state = STATE_NAME;
		drawrectsdl(props, 0xFFFFFFFF, 160, 190, 320, 100);
		drawframesdl(props, 0xFFFF0000, 16, 160, 190, 480, 290);
		writetextsdl(props, 0xFF000000, 192, 226, FONT, 32, "Name: ");
	}
	
	while(state != STATE_QUIT) {
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
						//if(newname == 0) {
							scoreList[MAX_LINES+1].name[newname] = (char)event.key.keysym.sym;
						//} else {
						//	sprintf(scoreList[MAX_LINES+1].name, "%s%c", scoreList[MAX_LINES+1].name, (char)event.key.keysym.sym);
						//}
						writetextsdl(props, 0xFF000000, 265, 226, FONT, 32, scoreList[MAX_LINES+1].name);
						
						newname++;
					}
				}
				if(event.key.keysym.sym == SDLK_RETURN || newname == MAX_NAME) {
					//scoreList[MAX_LINES+1] 
					printf("Name: %s\n", scoreList[MAX_LINES+1].name);
					state = STATE_QUIT;
				}
				if(event.key.keysym.sym == SDLK_BACKSPACE) {
					// draw white rectangle over the text
					drawrectsdl(props, 0xFFFFFFFF, 265, 226, 199, 34);
					if(newname > 0) {
						newname--;
						scoreList[MAX_LINES+1].name[newname] = 0;
						writetextsdl(props, 0xFF000000, 265, 226, FONT, 32, scoreList[MAX_LINES+1].name);
					}
					
					//newname--;
				}
			}
		}
		
		
	}
	
	//SDL_Delay(4000);
	
	printf("Hier ist die Score-Funktion!\n");
	printf("Score: %d\n", props->score);
	
	return -1;
}

