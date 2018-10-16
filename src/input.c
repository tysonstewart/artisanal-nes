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
		if (cursor.palette == 2) { // gem swap mode is ON
			cursor.swap_direction = PAD_LEFT;
			cursor.palette = 1;
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
		if (cursor.palette == 2) { // gem swap mode is ON
			cursor.swap_direction = PAD_RIGHT;
			cursor.palette = 1;
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
	// if(input&PAD_START){
	// 	input_btn_start();
	// 	return;
	// }
}