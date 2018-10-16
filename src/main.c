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
					gem_board.gem_state = GEM_STATE_SWAPPING;
					gem_board.swap_step = 0;
					gem_board.frame_counter = 0;
					// oam_clear();
				}
				break;
				
			case GEM_STATE_SWAPPING:
				write_debug("SWAPPING        ");
				if (gem_board.frame_counter <= SWAP_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (gem_board.swap_step <= 32) {
					animate_swap();
					gem_board.swap_step++;
					gem_board.frame_counter = 0;
				} else {
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
				fill_removed(); //Fills our internal state, but don't render them yet
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
					remove_matched();
					gem_board.gem_state = GEM_STATE_CLEARED;
					gem_board.frame_counter = 0;
				} else {
					gem_board.gem_state = GEM_STATE_FILLING;
					gem_board.frame_counter = 0;
				}
				break;
			
			case GEM_STATE_FILLING:
				write_debug("FILLING         ");
				if (gem_board.frame_counter <= FILL_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (check_filling()) {
					draw_falling_gems();
					gem_board.frame_counter = 0;
				} else {
					oam_clear();
					gem_board.gem_state = GEM_STATE_FILLED;
					gem_board.frame_counter = 0;
				}

			case GEM_STATE_FILLED:
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (check_any_matches()) {
					remove_matched();
					gem_board.gem_state = GEM_STATE_CLEARED;
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