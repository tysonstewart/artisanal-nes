#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct cursor_struct {
	unsigned char gem_x; //The gem position that the cursor should be over
	unsigned char gem_y;

	unsigned char palette; //The palette to use for the cursor. Used to highlight swap mode
	unsigned char new_render; //Set true to render a new cursor position
};

#endif