#include "consts.h"
#include "globals.h"
#include "neslib.h"

unsigned char valid_pop_color(unsigned char x, unsigned char y, unsigned char color){
    if (x < 2 && y < 2) {
        return TRUE;
    }
    if (x >= 2 && color == gem_board.gems[x-1][y] && color == gem_board.gems[x-2][y]){
        return FALSE;
    }
    if (y >= 2 && color == gem_board.gems[x][y-1] && color == gem_board.gems[x][y-2]){
        return FALSE;
    }
    return TRUE;
}

void populate_gem_colors(void) {
    //TODO seed rand function
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            //We'll pick a number, then check to make sure it didn't result in a match.
            //If it did, we'll pick another number.
            while(1) { //Only 2 colors could possibly cause a match, so there should always be an option that doesn't
                color_candidate = rand8()&0x03;
                if (valid_pop_color(x, y, color_candidate)){
                    gem_board.gems[x][y] = color_candidate;
                    break;
                }
            }
        }
    }
}