#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include "graphics.h"
#include "color.h"
#include "../util/util.h"

void graphics_clear(SDL_Surface* sf)
{
	SDL_FillRect(sf, NULL, 0x000000);
}

void graphics_setPixel(SDL_Surface* sf, int x, int y, color_t c)
{
	if (x >= 0 && x < sf->w && y >= 0 && y < sf->h)
	{
		Uint32 *pixels = (Uint32*) sf->pixels;
		int offset = (sf->pitch / sizeof(Uint32)) * y + x;
		*(pixels + offset) = SDL_MapRGBA(sf->format, RED(c), GREEN(c), BLUE(c), 0xFF);
	}
}

void graphics_vline(SDL_Surface* sf, int x, int y0, int y1, color_t c)
{
	if (x < 0 || x >= sf->w)
		return;
	for (int y=max(min(y0, y1), 0); y<min(max(y0, y1), sf->h); y++)
		graphics_setPixel(sf, x, y, c);
}

void graphics_hline(SDL_Surface* sf, int y, int x0, int x1, color_t c)
{
	if (y < 0 || y >= sf->h)
		return;
	for (int x=max(min(x0, x1), 0); x<min(max(x0, x1), sf->w); x++)
		graphics_setPixel(sf, x, y, c);
}

void graphics_line(SDL_Surface* sf, int x0, int y0, int x1, int y1, color_t c)
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	while(x0 != x1 && y0 != y1)
	{
		graphics_setPixel(sf,x0,y0,c);
		e2 = err;
		if (e2 >-dx)
			{err -= dy; x0 += sx;}
		if (e2 < dy)
			{err += dx; y0 += sy;}
	}
}
