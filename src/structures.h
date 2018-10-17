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
	char gems[GEM_BOARD_WIDTH+1][GEM_BOARD_HEIGHT+1];
	char board_copy[GEM_BOARD_WIDTH+1][GEM_BOARD_HEIGHT+1]; //TODO may want to find a more efficient way to track this
	char logos_to_update[GEM_BOARD_WIDTH+1][GEM_BOARD_HEIGHT+1]; //For re-rerendering bg logos live
	unsigned char new_render;
	unsigned char gem_state;
	unsigned char frame_counter;

	unsigned char swapping_x;
	unsigned char swapping_y;
	unsigned char swapping_dir;
	unsigned char swap_step;

	unsigned char game_start;
	unsigned char paused;
	unsigned char combo_counter;
};

#endif