#ifndef SDLHELPER_H_
#define SDLHELPER_H_

#include "snake.h"

int initsdl(struct properties* props);
int addbmpsdl(struct properties* props, char *file,
				unsigned int xpos, unsigned int ypos,
				unsigned int xsize, unsigned int ysize);
		
void cleanupsdl(struct properties* props);


#endif // SDLHELPER_H_
