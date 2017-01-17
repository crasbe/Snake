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
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
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

int addbmpsdl(struct properties *props, char *file, 
				unsigned int xpos, unsigned int ypos,
				unsigned int xsize, unsigned int ysize) {
	SDL_Surface *surf;
	SDL_Texture *tex;
	
	SDL_Rect destR ;
	destR.x = xpos;
	destR.y = ypos;
	destR.w = xsize;
	destR.h = ysize;
	
	// load BMP into a new surface
	surf = SDL_LoadBMP(file); 
	if(surf == NULL) {
		printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
		return 1;
	}
	
	// create a texture from the surface
	tex = SDL_CreateTextureFromSurface(props->ren, surf);
	if(tex == NULL) {
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return 1;
	}
	
	// the surface is not needed anymore, so we can free it
	SDL_FreeSurface(surf);
	
	if(SDL_RenderCopy(props->ren, tex, NULL, &destR) != 0) {
		printf("SDL_RenderCopy Error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_RenderPresent(props->ren);
	
	return 0;
}

// cleanup in the end
void cleanupsdl(struct properties *props) {
	SDL_DestroyRenderer(props->ren);
	SDL_DestroyWindow(props->win);
	SDL_Quit();
}
