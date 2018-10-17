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

unsigned char check_any_matches(void) {
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            if (gem_board.gems[x][y] == BLANK_GEM_COLOR) {
                continue;
            }
            if (x <= GEM_BOARD_WIDTH-2 && 
                    gem_board.gems[x][y] == gem_board.gems[x+1][y] &&
                    gem_board.gems[x][y] == gem_board.gems[x+2][y]){
                return TRUE;
            }
            if (y <= GEM_BOARD_HEIGHT-2 && 
                    gem_board.gems[x][y] == gem_board.gems[x][y+1] &&
                    gem_board.gems[x][y] == gem_board.gems[x][y+2]){
                return TRUE;
            }
        }
    }
    return FALSE;
}

//Figure out what the new gems will be. We need to copy information about which gems were missing into the other
//matrix so we can know where to render falling gems.
void fill_removed(void) {
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            gem_board.board_copy[x][y] = 0;
        }
    }

    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            if (gem_board.gems[x][y] == BLANK_GEM_COLOR) {
                gem_board.gems[x][y] = rand8()&0x03;
                gem_board.board_copy[x][y] = 1;
                gem_board.logos_to_update[x][y] = 1;
            }
        }
    }
}

unsigned char check_needing_drawn(void) {
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            if (gem_board.logos_to_update[x][y] > 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

unsigned char check_filling(void) {
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            if (gem_board.board_copy[x][y] > 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void settle_after_remove(void) {
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=GEM_BOARD_HEIGHT; y>=0; y--){ //Start at the bottom and work your way up
            if (gem_board.gems[x][y] == BLANK_GEM_COLOR){
                y2 = y-1;
                while(y2 >= 0){
                    if (gem_board.gems[x][y2] != BLANK_GEM_COLOR) {
                        gem_board.gems[x][y] = gem_board.gems[x][y2];
                        gem_board.gems[x][y2] = BLANK_GEM_COLOR;
                        break;
                    }
                    y2--;
                }
            }
        }
    }
}

//Find the matched gems and remove them. Gems above fall down, leaving blanks (-1) in their place
void remove_matched(void) {
    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            gem_board.board_copy[x][y] = 0;
        }
    }

    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            if (gem_board.gems[x][y] == BLANK_GEM_COLOR){
                continue;
            }
            if (x <= GEM_BOARD_WIDTH-2 && 
                    gem_board.gems[x][y] == gem_board.gems[x+1][y] &&
                    gem_board.gems[x][y] == gem_board.gems[x+2][y]){
                gem_board.board_copy[x][y] = 1;
                gem_board.board_copy[x+1][y] = 1;
                gem_board.board_copy[x+2][y] = 1;
            }
            if (y <= GEM_BOARD_HEIGHT-2 && 
                    gem_board.gems[x][y] == gem_board.gems[x][y+1] &&
                    gem_board.gems[x][y] == gem_board.gems[x][y+2]){
                gem_board.board_copy[x][y] = 1;
                gem_board.board_copy[x][y+1] = 1;
                gem_board.board_copy[x][y+2] = 1;
            }
        }
    }

    for (x=0; x<=GEM_BOARD_WIDTH; x++){
        for(y=0; y<=GEM_BOARD_HEIGHT; y++){
            if (gem_board.board_copy[x][y] == 1) {
                gem_board.gems[x][y] = BLANK_GEM_COLOR;
                gem_board.new_render = TRUE;
            }
        }
    }
}

void perform_swap(void) {
    color_candidate = gem_board.gems[cursor.gem_x][cursor.gem_y];
    switch (cursor.swap_direction) {
        case PAD_LEFT:
            if (cursor.gem_x == 0){
                return;
            }
            gem_board.gems[cursor.gem_x][cursor.gem_y] = gem_board.gems[cursor.gem_x-1][cursor.gem_y];
            gem_board.gems[cursor.gem_x-1][cursor.gem_y] = color_candidate;
            cursor.gem_x -= 1;
            cursor.new_render = TRUE;
            return;
        case PAD_RIGHT:
            if (cursor.gem_x == GEM_BOARD_WIDTH){
                return;
            }
            gem_board.gems[cursor.gem_x][cursor.gem_y] = gem_board.gems[cursor.gem_x+1][cursor.gem_y];
            gem_board.gems[cursor.gem_x+1][cursor.gem_y] = color_candidate;
            cursor.gem_x += 1;
            cursor.new_render = TRUE;
            return;
        case PAD_UP:
            if (cursor.gem_y == 0){
                return;
            }
            gem_board.gems[cursor.gem_x][cursor.gem_y] = gem_board.gems[cursor.gem_x][cursor.gem_y-1];
            gem_board.gems[cursor.gem_x][cursor.gem_y-1] = color_candidate;
            cursor.gem_y -= 1;
            cursor.new_render = TRUE;
            return;
        case PAD_DOWN:
            if (cursor.gem_y == GEM_BOARD_HEIGHT){
                return;
            }
            gem_board.gems[cursor.gem_x][cursor.gem_y] = gem_board.gems[cursor.gem_x][cursor.gem_y+1];
            gem_board.gems[cursor.gem_x][cursor.gem_y+1] = color_candidate;
            cursor.gem_y += 1;
            cursor.new_render = TRUE;
            return;
        default:
            return;
    }
}