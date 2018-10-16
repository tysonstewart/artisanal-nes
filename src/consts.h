#ifndef CONSTS_H
#define CONSTS_H

/**
 * Specific tiles
 */
#define HUDL_1 0x40
#define HUDL_2 0x41
#define HUDL_3 0x42
#define HUDL_4 0x43

#define HUDL_5 0x50
#define HUDL_6 0x51
#define HUDL_7 0x52
#define HUDL_8 0x53

#define HUDL_9 0x60
#define HUDL_10 0x61
#define HUDL_11 0x62
#define HUDL_12 0x63

#define HUDL_13 0x70
#define HUDL_14 0x71
#define HUDL_15 0x72
#define HUDL_16 0x73

static unsigned char hudl_logo[] = {
    HUDL_1, HUDL_2, HUDL_3, HUDL_4,
    HUDL_5, HUDL_6, HUDL_7, HUDL_8,
    HUDL_9, HUDL_10, HUDL_11, HUDL_12,
    HUDL_13, HUDL_14, HUDL_15, HUDL_16
};

static unsigned char blank_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define CURSOR_BORDER_TOP_LEFT  0x44
#define CURSOR_BORDER_TOP       0x45
#define CURSOR_BORDER_LEFT      0x46

/**
 * Game configuration
 */
#define GEM_BOARD_START_X 2 //2 8 pixel blocks over
#define GEM_BOARD_START_Y 4 //4 8 pixel blocks down
#define GEM_WIDTH       32 //24 pixels wide
#define GEM_WIDTH_TILES 4  // 32 pixels / 8 pixels per tile
#define GEM_BOARD_WIDTH 6 //7 gems wide
#define GEM_BOARD_HEIGHT 5 //6 gems high
#define BLANK_GEM_COLOR  6

/**
 * States of the gem board that can happen after a swap.
 */
#define GEM_STATE_READY     0 //Ready for next swap
#define GEM_STATE_SWAPPED   1 //We've just swapped
#define GEM_STATE_CLEARED   2 //We've just cleared some gems
#define GEM_STATE_SETTLED   3 //We've made gems fall
#define GEM_STATE_FILLING   4 //We're in the middle of filling the board
#define GEM_STATE_FILLED    5 //We've filled in blank spaces

#define ACTION_DELAY 20 //Frames to wait between gem board actions
#define FILL_DELAY 5 //Frames to wait between falling gem movements

/**
 * Macro for calculating the attribute byte of a sprite. The bits of this byte
 * store data as followed (taken from NerdyNights tutorial):
 *
 * 76543210
 * |||   ||
 * |||   ++- Color Palette of sprite.  Choose which set of 4 from the 16 colors to use
 * |||
 * ||+------ Priority (0: in front of background; 1: behind background)
 * |+------- Flip sprite horizontally
 * +-------- Flip sprite vertically
 */
#define SPRITE_ATTR(flip_horz,flip_vert,prio,pal) ((flip_horz<<7) + (flip_vert<<6) + (prio<<5) + pal)

static unsigned char cursor_sprite[] = {
    0, 0, CURSOR_BORDER_TOP_LEFT, 0,
    GEM_WIDTH - 8, 0, CURSOR_BORDER_TOP_LEFT, 0,
    0, GEM_WIDTH - 8, CURSOR_BORDER_TOP_LEFT, 0,
    GEM_WIDTH - 8, GEM_WIDTH - 8, CURSOR_BORDER_TOP_LEFT, 0,
    128
};

static unsigned char gem_sprite[] = {
    0, 0, HUDL_1, 0, 8, 0, HUDL_2, 0, 16, 0, HUDL_3, 0, 24, 0, HUDL_4, 0,
    0, 8, HUDL_5, 0, 8, 8, HUDL_6, 0, 16, 8, HUDL_7, 0, 24, 8, HUDL_8, 0,
    0, 16, HUDL_9, 0, 8, 16, HUDL_10, 0, 16, 16, HUDL_11, 0, 24, 16, HUDL_12, 0,
    0, 24, HUDL_13, 0, 8, 24, HUDL_14, 0, 16, 24, HUDL_15, 0, 24, 24, HUDL_16, 0,
    128
};

#endif