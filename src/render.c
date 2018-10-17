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
	y = (y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8) - (11-fall_step);
	oam_meta_spr(x, y, 0, gem_sprite);
}

void draw_hudl_logo(unsigned int x, unsigned int y) {
    int i;
    for (i = 0; i < 4; i++) {
        vram_adr(NTADR_A(x, y + i));
        vram_write(hudl_logo + (i*4), 4);
    }
}

//Don't think this'll work
// void draw_all_hudl_logos_live(){
// 	unsigned int address;
// 	// unsigned char update[745];
// 	unsigned char msb, lsb;
// 	unsigned int idx=0;

// 	for(y=0; y<4*GEM_BOARD_HEIGHT; y++){
// 		msb = GEM_BOARD_START_X;
// 		lsb = GEM_BOARD_START_Y + (y/4) * GEM_WIDTH_TILES + y;
// 		address = NTADR_A(msb, lsb);
// 		all_logo_update[idx++] = MSB(address)|NT_UPD_HORZ;
//     	all_logo_update[idx++] = LSB(address);
//     	all_logo_update[idx++] = 4;
// 		for(x=0; x<4*GEM_BOARD_WIDTH; x++){
// 			if (gem_board.gems[x/4][y/4] == BLANK_GEM_COLOR || gem_board.board_copy[x/4][y/4] > 0){
// 				all_logo_update[idx++] = blank_gem[4*(y%4) + (x%4)];
// 			} else {
// 				all_logo_update[idx++] = hudl_logo[4*(y%4) + (x%4)];
// 			}
// 		}
// 	}
// 	all_logo_update[idx] = NT_UPD_EOF;
// 	set_vram_update(all_logo_update);
// }

unsigned char* get_update_logo_bytes(unsigned char gem_x, unsigned char gem_y) {
	unsigned int address;
	unsigned char update_bytes[28];
	unsigned char msb, lsb;
	unsigned int idx=0;
	unsigned char t_x, t_y;

	
	for(t_y=0; t_y<4; t_y++){
		msb = GEM_BOARD_START_X + gem_x * GEM_WIDTH_TILES;
		lsb = GEM_BOARD_START_Y + gem_y * GEM_WIDTH_TILES + t_y;
		address = NTADR_A(msb, lsb);
		update_bytes[idx++] = MSB(address)|NT_UPD_HORZ;
    	update_bytes[idx++] = LSB(address);
    	update_bytes[idx++] = 4;
		for(t_x=0; t_x<4; t_x++){
			update_bytes[idx++] = hudl_logo[4*t_y + t_x];
		}
	}
	return update_bytes;
}

/**
 * Update up to 5 logos.
 * return 1 if still needs to draw more
 */
// #define LOGOS_AT_ONCE 1
// void draw_logos_needing_drawn(){
// 	unsigned int idx=0;
// 	unsigned int i=0;
// 	unsigned char update[LOGOS_AT_ONCE*28 + 1];
// 	unsigned char* logo_update;

// 	for(x=0; x<=GEM_BOARD_WIDTH; x++){
// 		for(y=0; y<=GEM_BOARD_HEIGHT; y++){
// 			if (gem_board.logos_to_update[x][y] == 1){
// 				logo_update = get_update_logo_bytes(x, y);
// 				for(i=0; i<27; i++){
// 					update[idx++] = logo_update[i];
// 				}
// 				gem_board.logos_to_update[x][y] = 0;
// 			}
// 			break;
// 			// if(idx >= LOGOS_AT_ONCE*28){
// 			// 	break;
// 			// }
// 		}
// 		break;
// 		// if(idx >= LOGOS_AT_ONCE*28){
// 		// 	break;
// 		// }
// 	}
// 	if (idx > 0){
// 		update[idx] = NT_UPD_EOF;
// 		set_vram_update(update);
// 	}
// }

void draw_hudl_logo_live2(unsigned char gem_x, unsigned char gem_y) {
	unsigned char update[29];
	unsigned char* logo_update = get_update_logo_bytes(gem_x, gem_y);
	int i;
	
	for(i=0; i<27; i++){
		update[i] = logo_update[i];
	}
	
	update[28] = NT_UPD_EOF;
	set_vram_update(update);
}

void draw_hudl_logo_live(unsigned char gem_x, unsigned char gem_y) {
    unsigned int address;
    unsigned char msb, lsb;
    msb = GEM_BOARD_START_X + gem_x * GEM_WIDTH_TILES;
    lsb = GEM_BOARD_START_Y + gem_y * GEM_WIDTH_TILES;
    address = NTADR_A(msb, lsb);
    msb = MSB(address);
    lsb = LSB(address);
    gem_bg[0] = msb;
    gem_bg[1] = lsb;
    gem_bg[3] = msb;
    gem_bg[4] = lsb + 1;
    gem_bg[6] = msb;
    gem_bg[7] = lsb + 2;
    gem_bg[9] = msb;
    gem_bg[10] = lsb + 3;
    lsb += 0x20;
    gem_bg[12] = msb;
    gem_bg[13] = lsb;
    gem_bg[15] = msb;
    gem_bg[16] = lsb + 1;
    gem_bg[18] = msb;
    gem_bg[19] = lsb + 2;
    gem_bg[21] = msb;
    gem_bg[22] = lsb + 3;
    lsb += 0x20;
    gem_bg[24] = msb;
    gem_bg[25] = lsb;
    gem_bg[27] = msb;
    gem_bg[28] = lsb + 1;
    gem_bg[30] = msb;
    gem_bg[31] = lsb + 2;
    gem_bg[33] = msb;
    gem_bg[34] = lsb + 3;
    lsb += 0x20;
    gem_bg[36] = msb;
    gem_bg[37] = lsb;
    gem_bg[39] = msb;
    gem_bg[40] = lsb + 1;
    gem_bg[42] = msb;
    gem_bg[43] = lsb + 2;
    gem_bg[45] = msb;
    gem_bg[46] = lsb + 3;

    set_vram_update(gem_bg);
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

unsigned char* get_gem_pal_update_bytes(unsigned char x, unsigned char y){
	unsigned char update_bytes[2];
    unsigned char gem, gem_left, gem_right, bg_left, bg_right;
	
	gem = gem_board.gems[x][y];
    if (x == 0) {
        gem_left = 0;
    } else {
        gem_left = gem_board.gems[x-1][y];
    }
    if (x == GEM_BOARD_WIDTH) {
        gem_right = 0;
    } else {
        gem_right = gem_board.gems[x+1][y];
    }

	bg_left = (gem << 6) | (gem_left << 4) | (gem << 2) | (gem_left);
	bg_right = (gem_right << 6) | (gem << 4) | (gem_right << 2) | (gem);

	update_bytes[0] = bg_left;
	update_bytes[1] = bg_right;

    return update_bytes;
}

void set_gem_pal_live(unsigned char x, unsigned char y){
    unsigned int address;
    unsigned char *gem_bytes = get_gem_pal_update_bytes(x, y);
    unsigned char update[6];

	bg_pal_x = get_bg_pal_x(x, y);
	bg_pal_y = get_bg_pal_y(x, y);
	address = NTADR_A(bg_pal_x, bg_pal_y);
	
    update[0] = MSB(address)|NT_UPD_HORZ;
    update[1] = LSB(address);
    update[2] = 2;
    update[3] = gem_bytes[0];
    update[4] = gem_bytes[1];
	update[5] = NT_UPD_EOF;
	set_vram_update(update);
}

void set_all_gem_pals_live() {
	unsigned int c_x, c_y;
	unsigned int address;
	unsigned char *gem_bytes;
	unsigned char update[52];

	address = NTADR_A(8, 30); //Hardcoded for first byte of bg gem palettes

	update[0] = MSB(address)|NT_UPD_HORZ;
    update[1] = LSB(address);
    update[2] = 48;

	for(c_x=0; c_x<=GEM_BOARD_WIDTH; c_x++){
		for(c_y=0; c_y<=GEM_BOARD_HEIGHT; c_y++){
			gem_bytes = get_gem_pal_update_bytes(c_x,c_y);
			update[3 + (c_y*8)+c_x] = gem_bytes[0];
			if(c_x == GEM_BOARD_WIDTH){
				update[3 + (c_y*8)+c_x+1] = gem_bytes[1];
			}
		}
	}

	update[51] = NT_UPD_EOF;
	set_vram_update(update);
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
				
				gem_board.board_copy[x][y] += 2;
				draw_falling_gem_sprite(x, y, gem_board.board_copy[x][y], gem_board.gems[x][y]);
				if (gem_board.board_copy[x][y] >= 10) {
                    draw_hudl_logo_live(x, y);
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

void prep_animate_swap(unsigned char step){
	int swapping_with_x = gem_board.swapping_x;
	int swapping_with_y = gem_board.swapping_y;
	if (step == 0) {
		draw_blank_logo_no_update(gem_board.swapping_x, gem_board.swapping_y);
	} else if (step == 1) {
		switch (gem_board.swapping_dir){
			case PAD_LEFT: 		swapping_with_x--; break;
			case PAD_RIGHT: 	swapping_with_x++; break;
			case PAD_UP:		swapping_with_y--; break;
			case PAD_DOWN:		swapping_with_y++; break;
		}
		draw_blank_logo_no_update(swapping_with_x, swapping_with_y);
	}
}

void post_animate_bg(unsigned char step){
	int swapping_with_x = gem_board.swapping_x;
	int swapping_with_y = gem_board.swapping_y;
	if (step == 0) {
		set_gem_pal_live(gem_board.swapping_x, gem_board.swapping_y);
	} else if (step == 1) {
		switch (gem_board.swapping_dir){
			case PAD_LEFT: 		swapping_with_x--; break;
			case PAD_RIGHT: 	swapping_with_x++; break;
			case PAD_UP:		swapping_with_y--; break;
			case PAD_DOWN:		swapping_with_y++; break;
		}
		set_gem_pal_live(swapping_with_x, swapping_with_y);
	}
}

void post_animate_swap(unsigned char step){
	int swapping_with_x = gem_board.swapping_x;
	int swapping_with_y = gem_board.swapping_y;
	if (step == 0) {
		draw_hudl_logo_live2(gem_board.swapping_x, gem_board.swapping_y);
	} else if (step == 1) {
		switch (gem_board.swapping_dir){
			case PAD_LEFT: 		swapping_with_x--; break;
			case PAD_RIGHT: 	swapping_with_x++; break;
			case PAD_UP:		swapping_with_y--; break;
			case PAD_DOWN:		swapping_with_y++; break;
		}
		draw_hudl_logo_live2(swapping_with_x, swapping_with_y);
	}
}

void animate_swap(unsigned char animate_step){
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
			x -= animate_step;
			swapping_with_x--;
			break;
		case PAD_RIGHT:
			x += animate_step;
			swapping_with_x++;
			break;
		case PAD_UP:
			y -= animate_step;
			swapping_with_y--;
			break;
		case PAD_DOWN:
			y += animate_step;
			swapping_with_y++;
			break;
	}

	sprite_offset = oam_meta_spr(x, y, 0, gem_sprite);

	for (i=3; i<63; i+=4){
		gem_sprite[i] = SPRITE_ATTR(0,0,0, gem_board.gems[swapping_with_x][swapping_with_y]);
	}
	x = (swapping_with_x * GEM_WIDTH) + (GEM_BOARD_START_X * 8);
	y = (swapping_with_y * GEM_WIDTH) + (GEM_BOARD_START_Y * 8);
	switch (gem_board.swapping_dir){
		case PAD_LEFT:
			x += animate_step;
			break;
		case PAD_RIGHT:
			x -= animate_step;
			break;
		case PAD_UP:
			y += animate_step;
			break;
		case PAD_DOWN:
			y -= animate_step;
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
			if (gem_board.gems[x2][y2] == BLANK_GEM_COLOR || gem_board.board_copy[x2][y2] > 0) {
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