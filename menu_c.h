/****************************************
 * This header contains macros and
 * and function prototypes for menu_c.c.
 ****************************************/
#ifndef MENU_C_H_
#define MENU_C_H_

#include "snake.h"

/****************************************
 * Macros
 ***************************************/
// states of the main loop
#define STATE_RUN 0
#define STATE_GAME 1
#define STATE_SCORE 2
#define STATE_QUIT 3

// macros for the buttons
#define BT_NONE 0 // no button selected
#define BT_START 1
#define BT_LEN_INC 2
#define BT_LEN_DEC 3
#define BT_VEL_INC 4
#define BT_VEL_DEC 5
#define BT_SCORE 6
#define BT_QUIT 7

// color of the buttons and color when clicked
#define BT_COLOR 0xFF0000FF
#define BT_COLOR_HIGH 0xFFFF0000

// 'hard' sizes of the buttons
#define BT_SPACING 5
#define BT_FRAMEWIDTH 5
#define BT_FONTSIZE 32
#define BT_XSIZE 500
#define BT_YSIZE 50
#define BT_XPART BT_YSIZE // size of the + and - buttons
#define BT_XPOS ((props->x)/2)-(BT_XSIZE)/2
#define BT_START_YPOS 200

#define BT_LEN_YPOS (BT_START_YPOS+1*(BT_YSIZE+BT_SPACING))
#define BT_VEL_YPOS (BT_START_YPOS+2*(BT_YSIZE+BT_SPACING))
#define BT_SCORE_YPOS (BT_START_YPOS+3*(BT_YSIZE+BT_SPACING))
#define BT_QUIT_YPOS (BT_START_YPOS+4*(BT_YSIZE+BT_SPACING))

#define BT_DEC_XPOS BT_XPOS
#define BT_MID_XPOS BT_XPOS+BT_XPART
#define BT_INC_XPOS BT_XPOS+BT_XSIZE-BT_XPART-BT_FRAMEWIDTH

/****************************************
 * Function prototypes
 * **************************************/
unsigned int xy2button(struct properties* props, int x, int y);
unsigned int drawallbuttons(struct properties* props, unsigned int highbutton);

#endif // MENU_C_H_
