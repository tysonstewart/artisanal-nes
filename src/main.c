#include "neslib.h"
#include "render.c"
#include "input.c"
#include "init.c"
#include "../res/gem_background_nam.h"



void main(void)
{
	draw_title_screen();
	delay(120);
	pal_spr(menue_pal);

    ppu_off();
	//gem background
	vram_adr(NAMETABLE_A);
    vram_unrle(gem_background_nam);
	draw_gem_board();

	//Example code for randomly setting colors
	for(x=0; x<=GEM_BOARD_WIDTH; x++){
		for(y=0; y<=GEM_BOARD_HEIGHT; y++){
			set_gem_pal(x, y, rand8()&0x03);
		}
	}

	ppu_on_all();									//enable rendering
	init_gem_board();
	while(1)//infinite loop, title-gameplay
	{
		mainloop_handle_input();
		mainloop_render();
		//Wait for next frame
		ppu_wait_nmi();
	}
}