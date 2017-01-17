#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "snake.h"
#include "sdlhelper.h"

int initsdl(struct properties* props) {
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());	
		return 1;
	}
	
	// open the window. x- and y-positions are given in the struct,
	// the window position is not defined and the window opens in the
	// foreground. The window title is "Snake"
	props->win = SDL_CreateWindow("Snake", 
									SDL_WINDOWPOS_UNDEFINED, 
									SDL_WINDOWPOS_UNDEFINED,
									props->x, props->y, 
									SDL_WINDOW_SHOWN);
	if (props->win == NULL) {
		printf("SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	
	// create the renderer for the window with no special rendering driver,
	// hardware acceleration and VSync
	props->ren = SDL_CreateRenderer(props->win, -1, 
									SDL_RENDERER_ACCELERATED | 
									SDL_RENDERER_PRESENTVSYNC);
	if (props->ren == NULL) {
		SDL_DestroyWindow(props->win);
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	
	// everything went smoothly
	return 0;
}

// cleanup in the end
void cleanupsdl(struct properties *props) {
	SDL_DestroyRenderer(props->ren);
	SDL_DestroyWindow(props->win);
	SDL_Quit();
}
