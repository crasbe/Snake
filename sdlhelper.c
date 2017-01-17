#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "snake.h"
#include "sdlhelper.h"

// This function initializes SDL, creates window and renderer and sets
// the window icon (very important! :)).
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
	
	fillallsdl(props, 0xFFFFFFFF); // white background with no alpha
	
	
	// set the window icon
	SDL_Surface *iconsurface;
	
	// load the icon into a surface
	iconsurface = SDL_LoadBMP("img/snake.bmp");
	if(iconsurface == NULL) {
		printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_SetWindowIcon(props->win, iconsurface);
	SDL_FreeSurface(iconsurface); // the surface is not needed anymore
	
	// everything went smoothly
	return 0;
}

int addbmpsdl(struct properties *props, char *file, 
				unsigned int xpos, unsigned int ypos,
				unsigned int xsize, unsigned int ysize) {
	SDL_Surface *bmpsurface;
	SDL_Texture *bmptexture;
	
	// rectangle with the size and position of the BMP
	SDL_Rect bmprect;
	bmprect.x = xpos;
	bmprect.y = ypos;
	bmprect.w = xsize;
	bmprect.h = ysize;
	
	// load BMP into a new surface
	bmpsurface = SDL_LoadBMP(file); 
	if(bmpsurface == NULL) {
		printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
		return 1;
	}
	
	// create a texture from the surface
	bmptexture = SDL_CreateTextureFromSurface(props->ren, bmpsurface);
	if(bmptexture == NULL) {
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return 1;
	}
	
	// the surface is not needed anymore, so we can free it
	SDL_FreeSurface(bmpsurface);
	
	if(SDL_RenderCopy(props->ren, bmptexture, NULL, &bmprect) != 0) {
		printf("SDL_RenderCopy Error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_RenderPresent(props->ren);
	
	return 0;
}

int fillallsdl(struct properties *props, uint32_t argb) {
	// blue has to be rshifted 0 bits (rightmost in argb already)
	// green has to be rshifted 8 bits
	// red has to be rshifted 16 bits
	// alpha has to be rshifted 24 bits (leftmost in argb)
	// mask with 0xFF to keep only the rightmost 8 bits
	if(SDL_SetRenderDrawColor(props->ren, (argb>>16)&0xFF, (argb>>8)&0xFF, 
										argb&0xFF, (argb>>24)&0xFF) != 0) {
		printf("SDL_SetRenderDrawColor Error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_RenderClear(props->ren);
	SDL_RenderPresent(props->ren);
	
	return 0;
}

// cleanup in the end
void cleanupsdl(struct properties *props) {
	SDL_DestroyRenderer(props->ren);
	SDL_DestroyWindow(props->win);
	SDL_Quit();
}
