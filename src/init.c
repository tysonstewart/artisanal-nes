#include "consts.h"
#include "globals.h"
#include "structures.h"
#include "neslib.h"

void init_gem_board(void){
    cursor.gem_x = 0;
	cursor.gem_y = 0;
	cursor.palette = 2;
	cursor.new_render = TRUE;
}