#include "neslib.h"

#define CHR_OFFSET 0x20

static unsigned char updateListData[] = {
    0x20|NT_UPD_HORZ, 0x2d, 0x12, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, NT_UPD_EOF
};

// Write a short debug message to the screen
// `buf` must be 16 bytes and should avoid lowercase letters
// side-effect: turns display on (both sprites and background)
void write_debug(unsigned char *buf) {
    int i;
    set_vram_update(updateListData);
    for (i = 0; i < 16; i++) {
        updateListData[i+4] = buf[i] - CHR_OFFSET;
    }

    ppu_on_all();
}