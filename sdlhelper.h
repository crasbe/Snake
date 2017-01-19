#ifndef SDLHELPER_H_
#define SDLHELPER_H_

#include <stdint.h>

#include "snake.h"

// Return values of initsdl and addbmpsdl:
// 0: everything went fine
// 1: error

extern unsigned int initsdl(struct properties* props);
extern unsigned int addbmpsdl(struct properties* props, char *file,
								unsigned int xpos, unsigned int ypos,
								unsigned int xsize, unsigned int ysize);
extern unsigned int fillallsdl(struct properties* props, uint32_t argb);
extern unsigned int drawbordersdl(struct properties* props, uint32_t argb, unsigned int width);
extern unsigned int drawrectsdl(struct properties* props, uint32_t argb,
								unsigned int xpos, unsigned int ypos,
								unsigned int xsize, unsigned int ysize);
extern unsigned int drawframesdl(struct properties* props, uint32_t argb, unsigned int width,
									unsigned int x1, unsigned int y1,
									unsigned int x2, unsigned int y2);
extern void cleanupsdl(struct properties* props);


#endif // SDLHELPER_H_
