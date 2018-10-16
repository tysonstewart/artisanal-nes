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
    sprite_offset = oam_spr((cursor.gem_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8), 
							(cursor.gem_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8), 
							CURSOR_BORDER_TOP_LEFT, SPRITE_ATTR(0,0,0, cursor.palette), 0);
	sprite_offset = oam_spr((cursor.gem_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8) + GEM_WIDTH - 8, 
							(cursor.gem_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8), 
							CURSOR_BORDER_TOP_LEFT, SPRITE_ATTR(0,1,0, cursor.palette), sprite_offset);
	sprite_offset = oam_spr((cursor.gem_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8), 
							(cursor.gem_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8) + GEM_WIDTH - 8, 
							CURSOR_BORDER_TOP_LEFT, SPRITE_ATTR(1,0,0, cursor.palette), sprite_offset);
	sprite_offset = oam_spr((cursor.gem_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8) + GEM_WIDTH - 8, 
							(cursor.gem_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8) + GEM_WIDTH - 8, 
							CURSOR_BORDER_TOP_LEFT, SPRITE_ATTR(1,1,0, cursor.palette), sprite_offset);
}

void draw_hudl_logo(unsigned int x, unsigned int y) {
    int i;
    for (i = 0; i < 4; i++) {
        vram_adr(NTADR_A(x, y + i));
        vram_write(hudl_logo + (i*4), 4);
    }
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

void draw_gem_board(void){
    for (x=GEM_BOARD_START_X; x<=GEM_BOARD_WIDTH*GEM_WIDTH/8 + GEM_BOARD_START_X; x += GEM_WIDTH/8){
		for (y=GEM_BOARD_START_Y; y<=GEM_BOARD_HEIGHT*GEM_WIDTH/8 + GEM_BOARD_START_Y; y += GEM_WIDTH/8){
			draw_hudl_logo(x,y);
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
		update_gem_colors();
		ppu_on_all();
		gem_board.new_render = FALSE;
	}

}