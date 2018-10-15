#ifndef CONSTS_H
#define CONSTS_H

/**
 * Specific tiles
 */
#define HUDL_1 0x40
#define HUDL_2 0x41
#define HUDL_3 0x42
#define HUDL_4 0x50
#define HUDL_5 0x51
#define HUDL_6 0x52
#define HUDL_7 0x60
#define HUDL_8 0x61
#define HUDL_9 0x62
#define CURSOR_BORDER_TOP_LEFT  0x44
#define CURSOR_BORDER_TOP       0x45
#define CURSOR_BORDER_LEFT      0x46

/**
 * Game configuration
 */
#define GEM_BOARD_START_X 2 //2 8 pixel blocks over
#define GEM_BOARD_START_Y 3 //4 8 pixel blocks down
#define GEM_WIDTH       24 //24 pixels wide
#define GEM_BOARD_WIDTH 7 //8 gems wide

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


#endif