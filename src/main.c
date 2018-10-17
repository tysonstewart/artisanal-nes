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
	while (gem_board.game_start != 1) {
		mainloop_handle_input();
		ppu_wait_nmi();
	}

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
				mainloop_handle_input();
				if (cursor.swap_direction != NULL) {
					gem_board.gem_state = GEM_STATE_SWAPPING;
					gem_board.swap_step = 0;
					gem_board.frame_counter = 0;
					// oam_clear();
				}
				break;
				
			case GEM_STATE_SWAPPING:
				if (gem_board.frame_counter <= SWAP_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (gem_board.swap_step <= 1) { //first two steps are clearing bg logos
					prep_animate_swap(gem_board.swap_step);
					gem_board.swap_step++;
				} else if (gem_board.swap_step <= 9) {
					animate_swap((gem_board.swap_step-2)*4);
					gem_board.swap_step ++;
				} else if (gem_board.swap_step == 10) {
					perform_swap();
					set_gem_pal_live(gem_board.swapping_x, gem_board.swapping_y);
					gem_board.swap_step++;
				} else if (gem_board.swap_step <= 12) {
					post_animate_bg(gem_board.swap_step-11);
					gem_board.swap_step++;
				} else if (gem_board.swap_step <= 14) {
					post_animate_swap(gem_board.swap_step-13);
					gem_board.swap_step++;
				} else {
					oam_clear();
					gem_board.gem_state = GEM_STATE_SWAPPED;
					gem_board.frame_counter = 0;
				}

				break;

			
			case GEM_STATE_SWAPPED:
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
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				settle_after_remove();
				fill_removed(); //Fills our internal state for properly setting backgrounds
				gem_board.new_render = TRUE;
				// set_all_gem_pals_live(); //TODO can use this if we make a method to update logos

				gem_board.gem_state = GEM_STATE_SETTLED;
				gem_board.frame_counter = 0;
				break;

			case GEM_STATE_SETTLED:
				if (gem_board.frame_counter <= ACTION_DELAY) {
					gem_board.frame_counter++;
					break;
				}
				if (!check_filling() && check_any_matches()) {
					remove_matched();
					gem_board.gem_state = GEM_STATE_CLEARED;
					gem_board.frame_counter = 0;
				} else {
					gem_board.gem_state = GEM_STATE_FILLING;
					gem_board.frame_counter = 0;
				}
				break;
			
			case GEM_STATE_FILLING:
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
			
			case GEM_STATE_TESTING:
				// if (gem_board.frame_counter <= 20) {
				// 	gem_board.frame_counter++;
				// 	break;
				// }
				// draw_hudl_logo_live2(0, 0);
				// gem_board.logos_to_update[0][0]=0;
				// if (check_needing_drawn()){
				// 	// draw_logos_needing_drawn();
				// } else {
				// 	gem_board.gem_state = GEM_STATE_READY;
				// 	gem_board.frame_counter = 0;
				// }

				gem_board.gem_state = GEM_STATE_READY;
				gem_board.frame_counter = 0;
				
				break;

		}

		if (gem_board.paused) {
			toggle_pause_palette(TRUE);
			while (gem_board.paused) {
				mainloop_handle_input();
				ppu_wait_nmi();
			}
			toggle_pause_palette(FALSE);
		}

		if (gem_board.new_render == 1) {
			write_debug("                "); //Crazy magic
		}
		mainloop_render();
		//Wait for next frame
		ppu_wait_nmi();
	}
}