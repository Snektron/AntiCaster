#include <SDL2/SDL.h>
#include "../util/graphics.h"
#include "tile.h"

int tile_isCollidable(tile_t* t)
{
	if (!t)
		return 0;
	return t->flags & FLAG_COLLIDABLE;
}

int tile_isVisible(tile_t* t)
{
	if (!t)
		return 0;
	return t->flags & FLAG_VISIBLE;
}

void tile_renderSolid(SDL_Surface* sf, tile_t* t, int x, int down, int up, hit_t* hit)
{
	graphics_vline(sf, x, down, up, t->color - hit->side * 0x000022);
}
