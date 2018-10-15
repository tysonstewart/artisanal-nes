#include "neslib.h"
#include "../res/titlescreen_nam.h"
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

	vram_adr(NTADR_A(x, y));
	vram_put(HUDL_1);
	vram_adr(NTADR_A(x+1, y));
	vram_put(HUDL_2);
	vram_adr(NTADR_A(x+2, y));
	vram_put(HUDL_3);
	vram_adr(NTADR_A(x, y+1));
	vram_put(HUDL_4);
	vram_adr(NTADR_A(x+1, y+1));
	vram_put(HUDL_5);
	vram_adr(NTADR_A(x+2, y+1));
	vram_put(HUDL_6);
	vram_adr(NTADR_A(x, y+2));
	vram_put(HUDL_7);
	vram_adr(NTADR_A(x+1, y+2));
	vram_put(HUDL_8);
	vram_adr(NTADR_A(x+2, y+2));
	vram_put(HUDL_9);
}

void mainloop_render(void){

    if (cursor.new_render){
        draw_cursor();
        cursor.new_render = FALSE;
    }
	

}