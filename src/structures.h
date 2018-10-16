#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "consts.h"

#define SWAP_DIRECTION unsigned char

typedef struct cursor_struct {
	unsigned char gem_x; //The gem position that the cursor should be over
	unsigned char gem_y;

	unsigned char palette; //The palette to use for the cursor. Used to highlight swap mode
	unsigned char new_render; //Set true to render a new cursor position
	SWAP_DIRECTION swap_direction; // use consts NULL, PAD_UP, PAD_DOWN, PAD_LEFT, and PAD_RIGHT
};

typedef struct gem_board_struct {
	unsigned char gems[GEM_BOARD_WIDTH+1][GEM_BOARD_HEIGHT+1];
};

#endif