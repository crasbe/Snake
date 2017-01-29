#include <stdio.h>
#include <stdint.h>

#include "snake.h"
#include "sdlhelper.h"

#include "menu_c.h"

#define VEL_STEP 50
#define LEN_STEP 10

int menu(struct properties* props) {
	
	props->v = 250;
	props->l = 100;
	
	SDL_Event event;
	unsigned int state = STATE_RUN;
	//unsigned int ret = 0;
	unsigned int button = BT_NONE;
	
	// fill everything with background color
	fillallsdl(props, 0xFFFFFFFF);
	
	// draw the logo in the center of the frame
	addbmpsdl(props, "img/snake.bmp", ((props->x)/2)-(256/2), 0, 256, 256);
	
	// draw the buttons	
	drawallbuttons(props, BT_NONE);
	
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
		
		// react to Ctrl-C
		if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_c) {
			if((SDL_GetModState() & KMOD_CTRL) == KMOD_RCTRL ||
				(SDL_GetModState() & KMOD_CTRL) == KMOD_LCTRL) {
					state = STATE_QUIT;
					continue; // don't do anything else after this
			}
		}
		
		// Mouse actions
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			int x, y;
			if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				button = xy2button(props, x, y);
				//printf("Button: %d\n", button);
				drawallbuttons(props, button); // highlight the button, but do nothing yet
			}
		}
		if(event.type == SDL_MOUSEBUTTONUP) {
			// when the mousebutton lifted do something
			switch(button) {
				case BT_QUIT: 	state = STATE_QUIT; break;
				case BT_SCORE:	state = STATE_SCORE; break;
				case BT_START:	state = STATE_GAME; break;
				case BT_VEL_INC: if(props->v >= 1000-VEL_STEP) break;
								 props->v += VEL_STEP; break;
				case BT_VEL_DEC: if(props->v <= VEL_STEP) break;
								 props->v -= VEL_STEP; break;
				case BT_LEN_INC: if(props->l >= 500) break;
								 props->l += LEN_STEP; break;
				case BT_LEN_DEC: if(props->l <= LEN_STEP) break;
								 props->l -= LEN_STEP; break;
			}
			drawallbuttons(props, BT_NONE);
			continue;
		}
	}
	
	if(state == STATE_QUIT) {
		return -1;
	} else if(state == STATE_GAME) {
		return 1;
	} else if(state == STATE_SCORE) {
		props->score = 0; // no highscorescore reached
		return 2;
	}
	return 1;
}

// helper functions
/***********************************************************************
 * drawallbuttons:
 *   (Re-)draws all the buttons.
 * Arguments:
 *   struct properties* props -
 *   unsigned int highbutton - which button to draw highlighted
 * Return: unsigned int
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int drawallbuttons(struct properties* props, unsigned int highbutton) {
	// used to save the text for variable buttons
	char text[20];

	// draw background color over the variable texts of length and velocity
	drawrectsdl(props, 0xFFFFFFFF, BT_XPOS+BT_XPART+BT_FRAMEWIDTH, BT_LEN_YPOS+BT_FRAMEWIDTH, BT_XSIZE-2*BT_XPART-2*BT_FRAMEWIDTH, BT_YSIZE-2*BT_FRAMEWIDTH);
	drawrectsdl(props, 0xFFFFFFFF, BT_XPOS+BT_XPART+BT_FRAMEWIDTH, BT_VEL_YPOS+BT_FRAMEWIDTH, BT_XSIZE-2*BT_XPART-2*BT_FRAMEWIDTH, BT_YSIZE-2*BT_FRAMEWIDTH);
	
	
	// start button
	drawbuttonsdl(props, BT_XPOS, BT_START_YPOS, BT_XSIZE, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_START ? BT_COLOR_HIGH : BT_COLOR), "Start!", FONT, BT_FONTSIZE);
	
	// 'Length'-part of the length button
	sprintf(text, "Sieglaenge: %d", props->l);
	drawbuttonsdl(props, BT_MID_XPOS, BT_LEN_YPOS, BT_XSIZE-2*BT_XPART, BT_YSIZE, BT_FRAMEWIDTH, BT_COLOR, text, FONT, BT_FONTSIZE);
	
	// '-'-part of the length button
	drawbuttonsdl(props, BT_DEC_XPOS, BT_LEN_YPOS, BT_XPART+BT_FRAMEWIDTH, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_LEN_DEC ? BT_COLOR_HIGH : BT_COLOR), "-", FONT, BT_FONTSIZE*2);
	
	// '+'-part of the length button
	drawbuttonsdl(props, BT_INC_XPOS, BT_LEN_YPOS, BT_XPART+BT_FRAMEWIDTH, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_LEN_INC ? BT_COLOR_HIGH : BT_COLOR), "+", FONT, BT_FONTSIZE*2);
	
	// text-part of velocity button
	sprintf(text, "Schrittzeit (ms): %d", props->v);
	drawbuttonsdl(props, BT_MID_XPOS, BT_VEL_YPOS, BT_XSIZE-2*BT_XPART, BT_YSIZE, BT_FRAMEWIDTH, BT_COLOR, text, FONT, BT_FONTSIZE);
	
	// '-'-part of the velocity button
	drawbuttonsdl(props, BT_DEC_XPOS, BT_VEL_YPOS, BT_XPART+BT_FRAMEWIDTH, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_VEL_DEC ? BT_COLOR_HIGH : BT_COLOR), "-", FONT, BT_FONTSIZE*2);
	
	// '+'-part of the velocity button
	drawbuttonsdl(props, BT_INC_XPOS, BT_VEL_YPOS, BT_XPART+BT_FRAMEWIDTH, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_VEL_INC ? BT_COLOR_HIGH : BT_COLOR), "+", FONT, BT_FONTSIZE*2);
	
	// score button
	drawbuttonsdl(props, BT_XPOS, BT_SCORE_YPOS, BT_XSIZE, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_SCORE ? BT_COLOR_HIGH : BT_COLOR), "Highscoreliste", FONT, BT_FONTSIZE);
	
	// quit button
	drawbuttonsdl(props, BT_XPOS, BT_QUIT_YPOS, BT_XSIZE, BT_YSIZE, BT_FRAMEWIDTH, (highbutton == BT_QUIT ? BT_COLOR_HIGH : BT_COLOR), "Programm beenden", FONT, BT_FONTSIZE);
	
	return 0;
}

/***********************************************************************
 * xy2button:
 * 
 **********************************************************************/
unsigned int xy2button(struct properties* props, int x, int y) {
	if(x < 0 || y < 0) {
		return BT_NONE;
	}
	unsigned int ux = (unsigned int)x;
	unsigned int uy = (unsigned int)y;
	
	// if the coordinates are out of the button field, no button has been clicked on
	if(ux < BT_XPOS || ux > BT_XPOS+BT_XSIZE || 
	   uy < BT_START_YPOS || uy > BT_QUIT_YPOS+BT_YSIZE) {
		return BT_NONE;
	} else if(uy >= BT_START_YPOS && uy <= BT_START_YPOS+BT_YSIZE) {
		return BT_START;
	} else if(uy >= BT_LEN_YPOS && uy <= BT_LEN_YPOS+BT_YSIZE) {
		if(ux >= BT_DEC_XPOS && ux <= BT_DEC_XPOS+BT_XPART) {
			return BT_LEN_DEC;
		} else if(ux >= BT_INC_XPOS && ux <= BT_INC_XPOS+BT_XPART) {
			return BT_LEN_INC;
		} else {
			return BT_NONE;
		}
	} else if(uy >= BT_VEL_YPOS && uy <= BT_VEL_YPOS+BT_YSIZE) {
		if(ux >= BT_DEC_XPOS && ux <= BT_DEC_XPOS+BT_XPART) {
			return BT_VEL_DEC;
		} else if(ux >= BT_INC_XPOS && ux <= BT_INC_XPOS+BT_XPART) {
			return BT_VEL_INC;
		} else {
			return BT_NONE;
		}
	} else if(uy >= BT_SCORE_YPOS && uy <= BT_SCORE_YPOS+BT_YSIZE) {
		return BT_SCORE;
	} else if(uy >= BT_QUIT_YPOS && uy <= BT_QUIT_YPOS+BT_YSIZE) {
		return BT_QUIT;
	} else {
		printf("Error: wrong button coordinates!\n");
	}
	return BT_NONE;
	
}
