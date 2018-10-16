#ifndef GLOBALS_H
#define GLOBALS_H
#include "structures.h"

/** Controller input */
static unsigned char input;
/** Controller's previous state. Used for hold down actions */
static unsigned char previous_state;

/** Cursor struct used for holding info about the cursor */
static struct cursor_struct cursor;
/** Struct used for holding info about the gems */
static struct gem_board_struct gem_board;

/** Used for rendering sprites */
static unsigned char sprite_offset;

static unsigned char *bg_pal;
static unsigned char bg_pal_x;
static unsigned char bg_pal_y;

static unsigned int x, y;
static unsigned int x2, y2;
static unsigned char color_candidate;

#endif