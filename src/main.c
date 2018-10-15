#include "neslib.h"
#include "render.c"
#include "debug.c"

void main(void)
{
	draw_title_screen();
	write_debug("IT'S SOMETHING!");
	while(1)//infinite loop, title-gameplay
	{
	}
}