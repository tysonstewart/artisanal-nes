#include "neslib.h"
#include "render.c"
#include "input.c"
#include "init.c"
#include "debug.c"

void main(void)
{
	draw_title_screen();
	write_debug("IT'S SOMETHING!");
	// delay(120);
	pal_spr(menue_pal);
	draw_hudl_logo(2, 2);
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