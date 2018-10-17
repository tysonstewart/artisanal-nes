#include "neslib.h"
#include "globals.h"
#include "structures.h"
#include "consts.h"

void mainloop_handle_input(void){
	input = pad_trigger(0);		/* Reading controller 1 input */
	previous_state = pad_state(0);
	if (input&PAD_A) {
		cursor.palette = 2; // set gem swap mode ON
		cursor.new_render = TRUE;
		gem_board.game_start = 1;
	}
	else if (previous_state&PAD_A && cursor.palette == 2){
		cursor.palette = 2;
	}
	else {
		cursor.palette = 1; // set gem swap mode OFF
		cursor.new_render = TRUE;
		cursor.swap_direction = NULL;
	}

	if(input&PAD_LEFT){
		if (cursor.palette == 2 && cursor.gem_x > 0) { // gem swap mode is ON
			cursor.swap_direction = PAD_LEFT;
			cursor.palette = 1;
			gem_board.swapping_dir = PAD_LEFT;
			gem_board.swapping_x = cursor.gem_x;
			gem_board.swapping_y = cursor.gem_y;
			return;
		}

		if (cursor.gem_x == 0) {
			cursor.gem_x = GEM_BOARD_WIDTH;
		} else {
			cursor.gem_x -= 1;
		}
		cursor.new_render = TRUE;
		return;
	}
	if(input&PAD_RIGHT){
		if (cursor.palette == 2 && cursor.gem_x < GEM_BOARD_WIDTH) { // gem swap mode is ON
			cursor.swap_direction = PAD_RIGHT;
			cursor.palette = 1;
			gem_board.swapping_dir = PAD_RIGHT;
			gem_board.swapping_x = cursor.gem_x;
			gem_board.swapping_y = cursor.gem_y;
			return;
		}

		if (cursor.gem_x == GEM_BOARD_WIDTH) {
			cursor.gem_x = 0;
		} else {
			cursor.gem_x += 1;
		}
		cursor.new_render = TRUE;
		return;
	}
	if(input&PAD_UP){
		if (cursor.palette == 2) { // gem swap mode is ON
			cursor.swap_direction = PAD_UP;
			cursor.palette = 1;
			gem_board.swapping_dir = PAD_UP;
			gem_board.swapping_x = cursor.gem_x;
			gem_board.swapping_y = cursor.gem_y;
			return;
		}

		if (cursor.gem_y == 0) {
			cursor.gem_y = GEM_BOARD_HEIGHT;
		} else {
			cursor.gem_y -= 1;
		}
		cursor.new_render = TRUE;
		return;
	}
	if(input&PAD_DOWN){
		if (cursor.palette == 2) { // gem swap mode is ON
			cursor.swap_direction = PAD_DOWN;
			cursor.palette = 1;
			gem_board.swapping_dir = PAD_DOWN;
			gem_board.swapping_x = cursor.gem_x;
			gem_board.swapping_y = cursor.gem_y;
			return;
		}

		if (cursor.gem_y == GEM_BOARD_HEIGHT) {
			cursor.gem_y = 0;
		} else {
			cursor.gem_y += 1;
		}
		cursor.new_render = TRUE;
		return;
	}
	if(input&PAD_B){
		// gem_board.board_copy[0][0] = 0x00;
		// gem_board.logos_to_update[0][0] = 1;
		// for(x=0; x<=GEM_BOARD_WIDTH; x++){
		// 	for(y=0; y<=GEM_BOARD_HEIGHT; y++){
		// 		gem_board.board_copy[x][y] = rand8()&0x01;
		// 		gem_board.logos_to_update[x][y] = 1;
		// 	}
		// }
		// gem_board.gem_state = GEM_STATE_TESTING;
		// gem_board.frame_counter = 0;
	}
	if(input&PAD_START){
		if (gem_board.game_start == 1) {
			gem_board.paused = !gem_board.paused;
		} else {
			gem_board.game_start = 1;
			gem_board.paused = 0;
		}
		
		return;
	}
}