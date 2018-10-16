#include "neslib.h"
#include "render.c"
#include "input.c"
#include "init.c"
#include "gem_logic.c"
#include "../res/gem_background_nam.h"

#include "debug.c"

void main(void)
{
	draw_title_screen();
	delay(120);

	write_debug("IT'S SOMETHING!");
	pal_spr(menue_pal);

    ppu_off();
	init_gem_board();
	populate_gem_colors();
	//gem background
	vram_adr(NAMETABLE_A);
    vram_unrle(gem_background_nam);
	draw_gem_board();

	//enable rendering
	ppu_on_all();
	
	while (1)
	{
		mainloop_handle_input();
		switch (cursor.swap_direction) {
			case PAD_LEFT:
				write_debug("SWAP LEFT       ");
				perform_swap();
				break;
			case PAD_RIGHT:
				write_debug("SWAP RIGHT      ");
				perform_swap();
				break;
			case PAD_UP:
				write_debug("SWAP UP         ");
				perform_swap();
				break;
			case PAD_DOWN:
				write_debug("SWAP DOWN       ");
				perform_swap();
				break;
			// default:
				//write_debug("FREE MOVEMENT   ");
		}
		if (check_any_matches()){
			write_debug("MATCH DETECTED  ");
			remove_matched();
			settle_after_remove();
		}

		mainloop_render();
		//Wait for next frame
		ppu_wait_nmi();
	}
}