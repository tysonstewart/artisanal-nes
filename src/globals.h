#ifndef GLOBALS_H
#define GLOBALS_H
#include "structures.h"

/** Controller input */
static unsigned char input;
/** Controller's previous state. Used for hold down actions */
static unsigned char previous_state;

/** Cursor struct used for holding info about the cursor */
static struct cursor_struct cursor;

/** Used for rendering sprites */
static unsigned char sprite_offset;

#endif