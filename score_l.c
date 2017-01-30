#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "snake.h"
#include "sdlhelper.h"
#include "menu_c.h"

#define namelength 20           // max length of playername
#define scorelistlength 1000    //max length of scorelist
#define MAX 25                  // maximum line length
#define SCORESSHOWN 10

struct sScore{
    char name[namelength];
    int punkte;
};

void sort(struct sScore *highscorearray, int length){                                         //not tested, same as original code(score_e.c)
    struct sScore tmp;
    for (int i = 0; i < length - 1; ++i){

        for (int j = 0; j < length - i - 1; ++j){
            if (highscorearray[j].punkte < highscorearray[j + 1].punkte){
                tmp = highscorearray[j];
                highscorearray[j] = highscorearray[j+1];
                highscorearray[j+1] = tmp;
            }
        }
    }
}

void writeData(struct sScore *highscorearray, int length, FILE* highscorefile){              //works well
    for (int i = 0;i < length;i++){
        fprintf(highscorefile,"%i:",highscorearray[i].punkte);
        fprintf(highscorefile,"%s \n",highscorearray[i].name);
    }

    rewind(highscorefile);
}

void readData(struct sScore *highscorearray, FILE *highscorefile){                           //the problem ist to read the data
    char line[MAX];
    int count = 0;

    while (fgets(line, MAX, highscorefile) !=0){
        sscanf(line, "%i:%s",&highscorearray[count].punkte, highscorearray[count].name);
        count++;
    }

    rewind(highscorefile); // back to start

  }

int countScores(FILE *highscorefile){                                                       //works well
    int count = 0;
    char line[scorelistlength];

    if (!highscorefile){
        printf("Could not open highscore \n" );
        return -2;
    }

    while (fgets( line, scorelistlength, highscorefile ) != 0){
        count++;
    }

    rewind(highscorefile); // put curser back to start

    return count;
}

void writenewscore(struct sScore *highscorearray,char *Name, int length, int newscore){     // dont know couse of problems with the r/w functions
    highscorearray[length -1].punkte = newscore;
    strcpy(highscorearray[length -1].name,Name);

    sort(highscorearray, length+1);
}

unsigned int drawscore(struct properties* props, unsigned int highbutton, struct sScore *highscorearray) {                 //copyed and modified

	fillallsdl(props, 0xFFFFFFFF);

    for(int i=0;i<SCORESSHOWN;i++){
        char score = highscorearray[i].punkte;
        writetextsdl(props, 0xFF000000, 50, 76+32*i, FONT, 30,  highscorearray[i].name);
        writetextsdl(props, 0xFF000000, 550, 76+32*i, FONT, 30,  &score);


    }


	// back to menue button
	drawbuttonsdl(props, BT_XPOS, BT_QUIT_YPOS, BT_XSIZE, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_QUIT ? BT_COLOR_HIGH : BT_COLOR), "back to menue", FONT, BT_FONTSIZE);

	return 0;
}

unsigned int xy2buttons(struct properties* props, int x, int y) {                            //copyed and modified
	if(x < 0 || y < 0) {
		return BT_NONE;
	}
	unsigned int ux = (unsigned int)x;
	unsigned int uy = (unsigned int)y;

	// if the coordinates are out of the button field, no button has been clicked on
	if(ux < BT_XPOS || ux > BT_XPOS+BT_XSIZE ||
	   uy < BT_START_YPOS || uy > BT_QUIT_YPOS+BT_YSIZE) {
		return BT_NONE;
	} else if(uy >= BT_QUIT_YPOS && uy <= BT_QUIT_YPOS+BT_YSIZE) {
		return BT_QUIT;
	} else {
		printf("Error: wrong button coordinates!\n");
	}
	return BT_NONE;

}


int score(struct properties* props) {
	FILE *highscorefile = fopen("highscore_l.txt", "r+");


    char Name[namelength];
    int length;
    props->score = 20;                                                                      // need the real score form game.c
    //printf("Enter your name:  ");
    //scanf("%s",Name);

    if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_c) {
				if((SDL_GetModState() & KMOD_CTRL) == KMOD_RCTRL ||
					(SDL_GetModState() & KMOD_CTRL) == KMOD_LCTRL) {
						state = STATE_QUIT;
						continue; // don't do anything else after this
				}
			}
				if(event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') {
					printf("Entered char: %c\n", event.key.keysym.sym);
					if(newname < MAX_NAME) {
						// add new character to name array and write new name to screen
						if(newname == 0) {
							Name = (char)event.key.keysym.sym;
						} else {
							sprintf(Name, "%s%c", Name, (char)event.key.keysym.sym);
						}
						writetextsdl(props, 0xFF000000, 265, 226, FONT, 32, Name);

						newname++;
					}
				} else if(event.key.keysym.sym == SDLK_BACKSPACE) {
					// draw white rectangle over the text
					drawrectsdl(props, 0xFFFFFFFF, 265, 226, 199, 34);





    length = countScores(highscorefile) +1;
    if (length < 0){        // if the file could not be open end funktion
        printf("highscorefile isnt there");
        return -1;
    }

    struct sScore* highscorearray;
    highscorearray = (struct sScore*)malloc((length)*sizeof(struct sScore));

    readData(highscorearray, highscorefile);
    writenewscore(highscorearray, Name,length, props->score);
    writeData(highscorearray, length, highscorefile);

    props->v = 250;
	props->l = 100;

    SDL_Event event;
	unsigned int state = STATE_RUN;
	//unsigned int ret = 0;
	unsigned int button = BT_NONE;

	// draw the buttons
	drawscore(props, BT_NONE, highscorearray);

	while(state == STATE_RUN) {
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


		// Mouse actions
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			int x, y;
			if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				button = xy2buttons(props, x, y);
				printf("Button: %d\n", button);
				drawallbuttons(props, button); // highlight the button, but do nothing yet
			}
		}
		if(event.type == SDL_MOUSEBUTTONUP) {
			// when the mousebutton lifted do something
			switch(button) {
				case BT_QUIT: 	state = STATE_QUIT; break;

			}
			drawallbuttons(props, BT_NONE);
			continue;
		}
	}

	if(state == STATE_QUIT) {
		return 0;                                          /*return 1 if highscore is closed in normal way and everithing was working well */
	}
return -1;

}
