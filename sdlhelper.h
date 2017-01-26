#ifndef SDLHELPER_H_
#define SDLHELPER_H_

#include <stdint.h>

#include "snake.h"

// Return values of initsdl and addbmpsdl:
// 0: everything went fine
// 1: error

unsigned int initsdl(struct properties* props);

unsigned int addbmpsdl( struct properties* props, char *file, 
						unsigned int xpos, unsigned int ypos,
						unsigned int xsize, unsigned int ysize);		
unsigned int fillallsdl(struct properties* props, uint32_t argb);
unsigned int drawbordersdl(struct properties* props, uint32_t argb,
						   unsigned int width);
unsigned int drawrectsdl(struct properties* props, uint32_t argb,
						 unsigned int xpos, unsigned int ypos,
						 unsigned int xsize, unsigned int ysize);
unsigned int drawframesdl(struct properties* props, uint32_t argb, unsigned int width,
						  unsigned int x1, unsigned int y1,
						  unsigned int x2, unsigned int y2);
unsigned int writetextsdl(struct properties* props, uint32_t argb,
						  unsigned int xpos, unsigned int ypos,
						  char* font, unsigned int fontsize, char* text);
unsigned int drawbuttonsdl(struct properties* props, 
							unsigned int xpos, unsigned int ypos, 
							unsigned int xsize, unsigned int ysize, 
							unsigned int borderwidth, uint32_t argb, 
							char* text, char* fontname, unsigned int fontsize);
void cleanupsdl(struct properties* props);


#endif // SDLHELPER_H_
