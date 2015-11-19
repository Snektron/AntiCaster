#include <SDL2/SDL.h>
#include <stdio.h>
#include "../raycast.h"
#include "../graphics/graphics.h"
#include "../graphics/color.h"
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

void tile_renderSolid(SDL_Surface* sf, tile_t* t, tileRenderData_t* d)
{
	graphics_vline(sf, d->x, sf->h/2 - d->h/2, sf->h/2 + d->h/2, color_sub(t->color, d->hit->side * 0x222222));
}

void tile_renderMirror(SDL_Surface* sf, tile_t* t, tileRenderData_t* d)
{
	tile_renderSolid(sf, t, d);
	ray_t r;
	r.distance = d->distance;
	r.orig = d->hit->point;
	raycast_getSideNormal(d->hit->side, &r.startdir);
	vec2_addScale(&r.orig, &r.startdir, .01, &r.orig);
	vec2_reflect(d->dir, d->hit->side, &r.startdir);

	raycast_column(sf, &r, d->map, d->x, d->cx, d->rec);
}
