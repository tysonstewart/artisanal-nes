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
		switch(gem_board.gem_state){
			case GEM_STATE_READY:
				write_debug("READY           ");
				mainloop_handle_input();
				if (cursor.swap_direction != NULL) {
					perform_swap();
					gem_board.gem_state = GEM_STATE_SWAPPED;
					gem_board.frame_counter = 0;
				}
				break;
				
			
			case GEM_STATE_SWAPPED:
				write_debug("SWAPPED         ");
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (check_any_matches()){
					remove_matched();
					gem_board.gem_state = GEM_STATE_CLEARED;
					gem_board.frame_counter = 0;
				} else {
					gem_board.gem_state = GEM_STATE_READY; //TODO probably swap back
					gem_board.frame_counter = 0;
				}
				break;

			case GEM_STATE_CLEARED:
				write_debug("CLEARED         ");
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				settle_after_remove();
				gem_board.gem_state = GEM_STATE_SETTLED;
				gem_board.frame_counter = 0;
				break;

			case GEM_STATE_SETTLED:
				write_debug("SETTLED         ");
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (check_any_matches()) {
					gem_board.gem_state = GEM_STATE_SWAPPED;
					gem_board.frame_counter = 0;
				} else {
					fill_removed();
					gem_board.gem_state = GEM_STATE_FILLED;
					gem_board.frame_counter = 0;
				}
				break;
			
			case GEM_STATE_FILLED:
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (check_any_matches()) {
					gem_board.gem_state = GEM_STATE_SWAPPED;
					gem_board.frame_counter = 0;
				} else {
					gem_board.gem_state = GEM_STATE_READY;
					gem_board.frame_counter = 0;
				}
				break;

		}

		mainloop_render();
		//Wait for next frame
		ppu_wait_nmi();
	}
}