#ifndef SDLHELPER_H_
#define SDLHELPER_H_

#include "snake.h"

// Return values of initsdl and addbmpsdl:
// 0: everything went fine
// 1: error

int initsdl(struct properties* props);
int addbmpsdl(struct properties* props, char *file,
				unsigned int xpos, unsigned int ypos,
				unsigned int xsize, unsigned int ysize);
		
void cleanupsdl(struct properties* props);


#endif // SDLHELPER_H_
