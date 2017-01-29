#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "snake.h"
#include "sdlhelper.h"

// private functions
unsigned int setdrawcolor(SDL_Renderer* ren, uint32_t argb) {
	// blue has to be rshifted 0 bits (rightmost in argb already)
	// green has to be rshifted 8 bits
	// red has to be rshifted 16 bits
	// alpha has to be rshifted 24 bits (leftmost in argb)
	// mask with 0xFF to keep only the rightmost 8 bits
	if(SDL_SetRenderDrawColor(ren, (argb>>16)&0xFF, (argb>>8)&0xFF, 
								argb&0xFF, (argb>>24)&0xFF) != 0) {
		printf("SDL_SetRenderDrawColor Error: %s\n", SDL_GetError());
		return 1;
	} else
		return 0;
}


// regular functions

/***********************************************************************
 * initsdl:
 *   This function initializes SDL, opens the window,
 *   creates the renderer and sets the window icon (very important! :)).
 * Arguments:
 *   struct properties* props: a pointer to custom struct
 * Return:
 *   0: success
 *   1: failure
***********************************************************************/
unsigned int initsdl(struct properties* props) {
	// initialize TTF library
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return 1;
	}

	
	
	// initialize video-, timer- and event-subsystems
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
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

/***********************************************************************
 * addbmpsdl:
 *   This functions draws a BMP image in the window (overlay).
 * Arguments:
 *   struct properties* props: 	pointer to custom struct with window
 * 								information
 * 	 char* file: pointer to a string with the path
 *   unsigned int xpos: X-position of the image
 *   unsigned int ypos: Y-position of the image
 *   unsigned int xsize: X-size of the image (width)
 *   unsigned int ysize: Y-size of the image (height)
 * Return:
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int addbmpsdl(struct properties* props, char* file, 
				unsigned int xpos, unsigned int ypos,
				unsigned int xsize, unsigned int ysize) {
	if(xpos > props->x) {
		printf("Error: xpos out of range: %d > %d\n", xpos, props->x);
		return 1;
	}
	if(ypos > props->y) {
		printf("Error: ypos out of range %d > %d\n", ypos, props->y);
	}
	
	SDL_Surface *bmpsurface;
	SDL_Texture *bmptexture;
	
	// rectangle with the size and position of the BMP
	SDL_Rect bmprect;
	bmprect.x = xpos;
	bmprect.y = ypos;
	bmprect.w = xsize;		// TODO: read x and y size from BMP
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

/***********************************************************************
 * fillallsdl:
 *   Fills the whole window with one color.
 * Arguments:
 *   struct properties* props - pointer to custom struct with window info
 *   uint32_t argb - fillcolorolor: Alpha, Red, Green, Blue
 * Return:
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int fillallsdl(struct properties* props, uint32_t argb) {
	if(setdrawcolor(props->ren, argb) == 1) {
		return 1;
	}
	SDL_RenderClear(props->ren);
	SDL_RenderPresent(props->ren);
	
	return 0;
}

/***********************************************************************
 * drawboardersdl:
 *   Draws a border in the window area.
 * Arguments:
 *   struct properties* props - pointer to custom struct
 *   uint32_t argb - color of the border; Alpha, Red, Green, Blue
 *   unsigned int width - width of the border
 * Return:
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int drawbordersdl(struct properties* props, uint32_t argb, unsigned int width) {
	return drawframesdl(props, argb, width, 0, 0, props->x, props->y);
}

/***********************************************************************
 * drawframesdl:
 *   Draws a frame in the window area.
 * Arguments:
 *   struct properties* props - pointer to custom struct
 *   uint32_t argb - color of the border. Alpha, Red, Green, Blue
 *   unsigned int width - width of the border
 *   unsigned int x1 - X-Position of the upper left corner
 *   unsigned int y1 - Y-Position of the upper left corner
 *   unsigned int x2 - X-Position of the lower right corner
 *   unsigned int y2 - Y-Position of the lower left corner
 * Return:
 *   0: success
 *   1 or more: failure
 **********************************************************************/
unsigned int drawframesdl(struct properties* props, uint32_t argb, unsigned int width,
						unsigned int x1, unsigned int y1,
						unsigned int x2, unsigned int y2) {
	unsigned int ret = 0;
	
	// draw from upper left corner to lower left corner
	ret += drawrectsdl(props, argb, x1, y1, width, y2-y1);
	
	// draw from upper left corner to upper right corner
	ret += drawrectsdl(props, argb, x1, y1, x2-x1, width);
	
	// draw from lower left corner to lower right corner
	ret += drawrectsdl(props, argb, x1, y2-width, x2-x1, width);
	
	// draw from upper right corner to lower right corner
	ret += drawrectsdl(props, argb, x2-width, y1, width, y2-y1);
	
	return ret;
}

/***********************************************************************
 * drawrectsdl:
 *   Draws a rectangle.
 * Arguments:
 *   struct properties* props - you've guessed it...
 *   uint32_t argb - color; Alpha, Red, Green, Blue
 *   unsigned int xpos - X-Position
 *   unsigned int ypos - Y-Position
 *   unsigned int xsize - X-Size (width)
 *   unsigned int ysize - Y-Size (height)
 * Return:
 *   0: success
 *   1: Failure 
 **********************************************************************/
unsigned int drawrectsdl(struct properties* props, uint32_t argb,
						unsigned int xpos, unsigned int ypos,
						unsigned int xsize, unsigned int ysize) {
	if(setdrawcolor(props->ren, argb) == 1) {
		return 1;
	}
	
	SDL_Rect rect;
	rect.x = xpos;
	rect.y = ypos;
	rect.w = xsize;
	rect.h = ysize;
	SDL_RenderFillRect(props->ren, &rect);
	
	SDL_RenderPresent(props->ren);
	
	return 0;
}

/***********************************************************************
 * writetextsdl:
 *   Writes a specified text to the screen.
 * Arguments:
 *   struct properties* props - who knows...
 *   uint32_t argb - textcolor; Alpha, Red, Green, Blue
 *   unsigned int xpos - X-Position of the text
 *   unsigned int ypos - Y-Position of the text
 *   char* font - path to the truetype (.ttf) font file
 *   unsigned int fontsize - Fontsize
 *   char* text: the text to write
 * Return:
 *   0: success
 *   1: failure
 **********************************************************************/
unsigned int writetextsdl(struct properties* props, uint32_t argb,
						unsigned int xpos, unsigned int ypos, char* fontname,
						unsigned int fontsize, char* text) {
	int width, height;
	
	// load the font
	TTF_Font* font = TTF_OpenFont(fontname, fontsize);
	if(font == NULL) {
		printf("TTF_Font Error: %s\n", TTF_GetError());
		return 1;
	}
	
	// Calculate the size of the necessary box
	if(TTF_SizeText(font, text, &width, &height) != 0) {
		printf("TTF_SizeText Error: %s\n", TTF_GetError());
		return 1;
	}
	// specify text color
	SDL_Color color = { (argb>>16)&0xFF, (argb>>8)&0xFF, argb&0xFF, (argb>>24)&0xFF };
	
	// generate surface for the text
	SDL_Surface* sur = TTF_RenderText_Solid(font, text, color);
	if(sur == NULL) {
		printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
		return 1;
	}
	
	// Font is not needed anymore
	TTF_CloseFont(font);
	
	// generate texture from the surface
	SDL_Texture* tex = SDL_CreateTextureFromSurface(props->ren, sur);
	if(tex == NULL) {
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return 1;
	}

	// the surface is no longer needed
	SDL_FreeSurface(sur);

	// specify rectangle for the text
	SDL_Rect rect;
	rect.x = xpos;
	rect.y = ypos;
	rect.w = width;
	rect.h = height;
	
	// render the text into the buffer
	if(SDL_RenderCopy(props->ren, tex, NULL, &rect) != 0) {
		printf("SDL_RenderCopy Error: %s\n", SDL_GetError());
		return 1;
	}
	
	// copy buffer onto screen
	SDL_RenderPresent(props->ren);
	
	return 0;
}

/***********************************************************************
 * drawbuttonsdl:
 *   Draw a button with text.
 * Arguments:
 *   struct properties* props -
 *   unsigned int xpos - X-position
 *   unsigned int ypos - Y-position
 *   unsigned int xsize - X-size
 *   unsigned int ysize - Y-size
 *   unsigned int borderwidth - Border width
 *   uint32_t argb - color in Alpha, Red, Green, Blue
 *   char* text
 *   char* fontname
 *   unsigned int fontsize
 * Return:
 *   0 - success
 *   1 - failure
 ***********************************************************************/				
unsigned int drawbuttonsdl(struct properties* props, 
							unsigned int xpos, unsigned int ypos, 
							unsigned int xsize, unsigned int ysize, 
							unsigned int borderwidth, uint32_t argb, 
							char* text, char* fontname, unsigned int fontsize) {
	// draw the frame around the button
	if(drawframesdl(props, argb, borderwidth, xpos, ypos, xpos+xsize, ypos+ysize) != 0) {
		return 1;
	}
	
	// calculate the size of the text
	int textwidth, textheight;
	
	TTF_Font *fontpointer = TTF_OpenFont(fontname, fontsize);
	
	if(TTF_SizeText(fontpointer, text, &textwidth, &textheight) != 0) {
		printf("TTF_SizeText Error: %s\n", TTF_GetError());
		return 1;
	}
	TTF_CloseFont(fontpointer);
	
	if(writetextsdl(props, argb, xpos+(xsize/2)-(textwidth/2), ypos+(ysize/2)-(textheight/2), fontname, fontsize, text) != 0) {
		return 1;
	}
	
	return 0;
}

/***********************************************************************
 * cleanupsdl:
 *   Partying is fun but someone has to clean up the mess.
 * Arguments:
 *   struct properties* props - yeah...
 * Return:
 *   void 
 **********************************************************************/
void cleanupsdl(struct properties *props) {
	SDL_DestroyRenderer(props->ren);
	SDL_DestroyWindow(props->win);
	SDL_Quit();
}
