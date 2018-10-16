#include "neslib.h"
#include "../res/titlescreen_nam.h"
#include "../res/gem_background_nam.h"
#include "../res/menue_pal.h"
#include "consts.h"
#include "globals.h"

void draw_title_screen() {
    ppu_off();
    oam_clear();

    vram_adr(NAMETABLE_A);
    vram_unrle(titlescreen_nam);
    pal_bg(menue_pal);
    
    ppu_on_bg();
}

void draw_cursor(void) {
    cursor_sprite[3] = SPRITE_ATTR(0,0,0, cursor.palette);
    cursor_sprite[7] = SPRITE_ATTR(0,1,0, cursor.palette);
    cursor_sprite[11] = SPRITE_ATTR(1,0,0, cursor.palette);
    cursor_sprite[15] = SPRITE_ATTR(1,1,0, cursor.palette);
    x = (cursor.gem_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8);
    y = (cursor.gem_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8);
    oam_meta_spr(x, y, 0, cursor_sprite);
}

void draw_falling_gem_sprite(unsigned int x, unsigned int y, unsigned int fall_step, unsigned char pal) {
	int i = 0;
	for (i=3; i<63; i+=4){
		gem_sprite[i] = SPRITE_ATTR(0,0,0, pal);
	}
	x = (x * GEM_WIDTH) + (GEM_BOARD_START_X * 8);
	y = (y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8) - (10-fall_step);
	oam_meta_spr(x, y, 0, gem_sprite);
}

void draw_hudl_logo(unsigned int x, unsigned int y) {
    int i;
    for (i = 0; i < 4; i++) {
        vram_adr(NTADR_A(x, y + i));
        vram_write(hudl_logo + (i*4), 4);
    }
}

void draw_blank_logo(unsigned int x,  unsigned int y) {
	int i;
    for (i = 0; i < 4; i++) {
        vram_adr(NTADR_A(x, y + i));
        vram_write(blank_logo + (i*4), 4);
    }
}

void draw_blank_logo_no_update(unsigned int x, unsigned int y) {
	unsigned int address;
    x = GEM_BOARD_START_X + x * GEM_WIDTH_TILES;
    y = GEM_BOARD_START_Y + y * GEM_WIDTH_TILES;
    address = NTADR_A(x, y);

    blank_gem[0] = MSB(address)|NT_UPD_HORZ;
    blank_gem[1] = LSB(address);
    address += 0x20;
    blank_gem[7] = MSB(address)|NT_UPD_HORZ;
    blank_gem[8] = LSB(address);
    address += 0x20;
    blank_gem[14] = MSB(address)|NT_UPD_HORZ;
    blank_gem[15] = LSB(address);
    address += 0x20;
    blank_gem[21] = MSB(address)|NT_UPD_HORZ;
    blank_gem[22] = LSB(address);
    set_vram_update(blank_gem);
}

/**
 * These two methods make assumptions about gem size and where the board starts
 * starting background palette is at x=8, y=30 with most sig bits being left half of first gem
 */
unsigned char get_bg_pal_x(unsigned char gem_x, unsigned char gem_y){
	switch(gem_y){
		case 0: 
		case 4:
			return gem_x + 8;
		case 1:
		case 5:
			return gem_x + 16;
		case 2:
		case 6:
			return gem_x + 24;
		case 3:
			return gem_x;
	}
}

unsigned char get_bg_pal_y(unsigned char gem_x, unsigned char gem_y){
	if (gem_y < 3) {
		return 30;
	} else {
		return 31;
	}
}

void set_gem_pal(unsigned char x, unsigned char y, unsigned char pal){
	bg_pal_x = get_bg_pal_x(x, y);
	bg_pal_y = get_bg_pal_y(x, y);
	vram_adr(NTADR_A(bg_pal_x, bg_pal_y));
	vram_read(bg_pal, 1);

	*bg_pal = *bg_pal & 0x33;
	if (pal == 0){
		*bg_pal = *bg_pal | 0x00;
	} else if (pal == 1) {
		*bg_pal = *bg_pal | 0x44;
	} else if (pal == 2) {
		*bg_pal = *bg_pal | 0x88;
	} else if (pal == 3) {
		*bg_pal = *bg_pal | 0xCC;
	}
	vram_adr(NTADR_A(bg_pal_x, bg_pal_y));
	vram_put(*bg_pal);
	
	vram_adr(NTADR_A(bg_pal_x+1, bg_pal_y));
	vram_read(bg_pal, 1);

	*bg_pal = *bg_pal & 0xCC;
	if (pal == 0){
		*bg_pal = *bg_pal | 0x00;
	} else if (pal == 1) {
		*bg_pal = *bg_pal | 0x11;
	} else if (pal == 2) {
		*bg_pal = *bg_pal | 0x22;
	} else if (pal == 3) {
		*bg_pal = *bg_pal | 0x33;
	}
	vram_adr(NTADR_A(bg_pal_x+1, bg_pal_y));
	vram_put(*bg_pal);
}

void update_gem_colors(void){
	for (x=0; x<=GEM_BOARD_WIDTH; x++){
		for(y=0; y<=GEM_BOARD_HEIGHT; y++){
			set_gem_pal(x, y, gem_board.gems[x][y]);
		}
	}
}

void draw_falling_gems(void){
	unsigned char gotit = 0;
	//info about which gems are falling should be in the board copy
	oam_clear();
	
	for (x=0; x<=GEM_BOARD_WIDTH; x++){
		for(y=GEM_BOARD_HEIGHT; y>=0; y--){ //start at the bottom
			if (gem_board.board_copy[x][y] > 0){
				gem_board.board_copy[x][y] += 1;
				draw_falling_gem_sprite(x, y, gem_board.board_copy[x][y], gem_board.gems[x][y]);
				if (gem_board.board_copy[x][y] >= 10) {
					gem_board.board_copy[x][y] = 0;
				}
				gotit = 1;
				break;
			}
		}
		if (gotit){
			break;
		}
	}
}

void animate_swap(void){
	int i = 0;
	int swapping_with_x = gem_board.swapping_x;
	int swapping_with_y = gem_board.swapping_y;

	

	for (i=3; i<63; i+=4){
		gem_sprite[i] = SPRITE_ATTR(0,0,0, gem_board.gems[gem_board.swapping_x][gem_board.swapping_y]);
	}
	x = (gem_board.swapping_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8);
	y = (gem_board.swapping_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8);
	switch (gem_board.swapping_dir){
		case PAD_LEFT:
			x -= gem_board.swap_step;
			swapping_with_x--;
			break;
		case PAD_RIGHT:
			x += gem_board.swap_step;
			swapping_with_x++;
			break;
		case PAD_UP:
			y -= gem_board.swap_step;
			swapping_with_y--;
			break;
		case PAD_DOWN:
			y += gem_board.swap_step;
			swapping_with_y++;
			break;
	}

	if (gem_board.swap_step == 0) {
		draw_blank_logo_no_update(gem_board.swapping_x, gem_board.swapping_y);
	}
	if (gem_board.swap_step == 2) {
		draw_blank_logo_no_update(swapping_with_x, swapping_with_y);
	}
	if (gem_board.swap_step >= 32) {
		//TODO update these live instead of needing a rerender
		oam_clear();
		draw_hudl_logo(gem_board.swapping_x, gem_board.swapping_y);
		draw_hudl_logo(swapping_with_x, swapping_with_y);
		return;
	}

	sprite_offset = oam_meta_spr(x, y, 0, gem_sprite);

	for (i=3; i<63; i+=4){
		gem_sprite[i] = SPRITE_ATTR(0,0,0, gem_board.gems[swapping_with_x][swapping_with_y]);
	}
	x = (swapping_with_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8);
	y = (swapping_with_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8);
	switch (gem_board.swapping_dir){
		case PAD_LEFT:
			x += gem_board.swap_step;
			break;
		case PAD_RIGHT:
			x -= gem_board.swap_step;
			break;
		case PAD_UP:
			y += gem_board.swap_step;
			break;
		case PAD_DOWN:
			y -= gem_board.swap_step;
			break;
	}
	sprite_offset = oam_meta_spr(x, y, sprite_offset, gem_sprite);
}

void draw_gem_board(void){
	// for (x = 0; x <= GEM_BOARD_WIDTH; x++){
	// 	for (y = 0; y <= GEM_BOARD_HEIGHT; y++){
    //         draw_hudl_logo(GEM_BOARD_START_X + x * GEM_WIDTH_TILES, 
    //                        GEM_BOARD_START_Y + y * GEM_WIDTH_TILES);
	// 	}
	// }
    for (x=GEM_BOARD_START_X; x<=GEM_BOARD_WIDTH*GEM_WIDTH/8 + GEM_BOARD_START_X; x += GEM_WIDTH/8){
		for (y=GEM_BOARD_START_Y; y<=GEM_BOARD_HEIGHT*GEM_WIDTH/8 + GEM_BOARD_START_Y; y += GEM_WIDTH/8){
			x2 = (x - GEM_BOARD_START_X) / 4;
			y2 = (y - GEM_BOARD_START_Y) / 4;
			if (gem_board.gems[x2][y2] == BLANK_GEM_COLOR) {
				draw_blank_logo(x,y);
			} else {
				draw_hudl_logo(x,y);
			}
		}
	}
	update_gem_colors();	
}

void mainloop_render(void){

    if (cursor.new_render){
        draw_cursor();
        cursor.new_render = FALSE;
    }
	if (gem_board.new_render){
		ppu_off();
		draw_gem_board();
		ppu_on_all();
		gem_board.new_render = FALSE;
	}

}