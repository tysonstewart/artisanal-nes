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

	populate_gem_colors();
	//gem background
	vram_adr(NAMETABLE_A);
    vram_unrle(gem_background_nam);
	draw_gem_board();

	//enable rendering
	ppu_on_all();
	init_gem_board();
	while (1)
	{
		mainloop_handle_input();
		switch (cursor.swap_direction) {
			case PAD_LEFT:
				write_debug("SWAP LEFT       ");
				break;
			case PAD_RIGHT:
				write_debug("SWAP RIGHT      ");
				break;
			case PAD_UP:
				write_debug("SWAP UP         ");
				break;
			case PAD_DOWN:
				write_debug("SWAP DOWN       ");
				break;
			default:
				write_debug("FREE MOVEMENT   ");
		}

		mainloop_render();
		//Wait for next frame
		ppu_wait_nmi();
	}
}