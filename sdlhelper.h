#ifndef SDLHELPER_H_
#define SDLHELPER_H_

#include <stdint.h>

#include "snake.h"

// Return values of initsdl and addbmpsdl:
// 0: everything went fine
// 1: error

extern int initsdl(struct properties* props);
extern int addbmpsdl(struct properties* props, char *file,
					unsigned int xpos, unsigned int ypos,
					unsigned int xsize, unsigned int ysize);
extern int fillallsdl(struct properties* props, uint32_t argb);
extern void cleanupsdl(struct properties* props);


#endif // SDLHELPER_H_
