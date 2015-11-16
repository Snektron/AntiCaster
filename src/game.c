#include <SDL2/SDL.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "game.h"
#include "util/graphics.h"
#include "math/vec2.h"
#include "map.h"

#define PLAYER_MOVESPEED 0.05
#define PLAYER_ROTSPEED 0.007

typedef struct
{
	vec2_t pos;
	vec2_t dir;
} player_t;

player_t p;
map_t m;

void game_init()
{
	p.pos = (vec2_t) {5, 5};
	p.dir = (vec2_t) {-1., 0.};

	block_t defaultWall = {0, 1., 1.};

	map_init(&m, 20, 20, &defaultWall);

	block_t bluewall = {1, 1., 1.};
	block_t greenwall = {2, 1., 1.};
	block_t redwall = {3, 1., 1.};
	block_t compressedWall = {0, 1., .1};

	for (int i=0; i<20; i++)
	{
		map_setBlockAt(&m, i, 0, &bluewall);
		map_setBlockAt(&m, i, 19, &bluewall);
		map_setBlockAt(&m, 0, i, &bluewall);
		map_setBlockAt(&m, 19, i, &bluewall);
	}

	map_setBlockAt(&m, 10, 10, &greenwall);
	map_setBlockAt(&m, 11, 10, &compressedWall);
	map_setBlockAt(&m, 12, 10, &compressedWall);
	map_setBlockAt(&m, 13, 10, &compressedWall);
	map_setBlockAt(&m, 14, 10, &greenwall);

	map_setBlockAt(&m, 11, 16, &redwall);
}

int game_update(int mousedx, int mousedy)
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	block_t* b = map_getBlockAt(&m, (int) p.pos.x,  (int) p.pos.y);

	vec2_rotate(&p.dir, mousedx*PLAYER_ROTSPEED, &p.dir);

	vec2_t dir = p.dir;
	vec2_t dirx = {dir.y, -dir.x};

	if (b)
	{
		dir.x *= b->spacex;
		dir.y *= b->spacey;
		dirx.x *= b->spacex;
		dirx.y *= b->spacey;
	}

	if (keys[SDL_SCANCODE_W])
		vec2_addScale(&p.pos, &dir, PLAYER_MOVESPEED, &p.pos);
	if (keys[SDL_SCANCODE_S])
		vec2_addScale(&p.pos, &dir, -PLAYER_MOVESPEED, &p.pos);
	if (keys[SDL_SCANCODE_A])
		vec2_addScale(&p.pos, &dirx, -PLAYER_MOVESPEED, &p.pos);
	if (keys[SDL_SCANCODE_D])
		vec2_addScale(&p.pos, &dirx, PLAYER_MOVESPEED, &p.pos);
	if (keys[SDL_SCANCODE_ESCAPE])
		return 1;
	return 0;
}

int sign(double x)
{
	return x < 0 ? -1 : 1;
}

int mapx(SDL_Surface* sf, double x)
{
	double vw = sf->h/20.;
	return (int)(sf->w/2.+vw*(x-10.));
}

int mapy(SDL_Surface* sf, double y)
{
	double vw = sf->h/20.;
	return (int)(sf->h/2.+vw*(y-10.));
}

typedef struct
{
	int mapX, mapY, side;
	vec2_t orig;
	vec2_t dir;
	double distance;
} ray_t;

typedef struct
{
	int hit;
	block_t* hitblock;
	int side;
	double distance;
	vec2_t point;
} hit_t;

void nextHit(ray_t* r)
{
	block_t* hb = map_getBlockAt(&m, r->mapX, r->mapY);
	vec2_t s = hb? (vec2_t){hb->spacex, hb->spacey} :  (vec2_t){1,1};

	vec2_t dir = r->dir;
	r->dir.x *= s.x;
	r->dir.y *= s.y;

	vec2_normalize(&r->dir, &r->dir);

	double deltaDistX = sqrt(1 + (r->dir.y * r->dir.y) / (r->dir.x * r->dir.x));
	double deltaDistY = sqrt(1 + (r->dir.x * r->dir.x) / (r->dir.y * r->dir.y));

	int stepX = sign(r->dir.x);
	int stepY = sign(r->dir.y);

	double sideDistX = (stepX < 0 ? (r->orig.x - r->mapX) : (r->mapX + 1.0 - r->orig.x)) * deltaDistX;
	double sideDistY = (stepY < 0 ? (r->orig.y - r->mapY) : (r->mapY + 1.0 - r->orig.y)) * deltaDistY;

	vec2_t d = {0,0};

	if (sideDistX < sideDistY)
	{
		d.x = r->dir.x * sideDistX;
		d.y = r->dir.y * sideDistX;
		r->mapX += stepX;
		r->side = 0;
	} else
	{
		d.x = r->dir.x * sideDistY;
		d.y = r->dir.y * sideDistY;
		r->mapY += stepY;
		r->side = 1;
	}
	r->orig.x += d.x;
	r->orig.y += d.y;
	r->distance += sqrt(pow(d.x/s.x, 2) + pow(d.y/s.y, 2));

	r->dir = dir;
}

// cast the ray.
void cast(SDL_Surface* sf, ray_t* r, hit_t* hit)
{
	hit->hit = 0;

	r->mapX = (int) r->orig.x;
	r->mapY = (int) r->orig.y;
	r->distance = 0;

	for (int _=0; _<100; _++)
	{
		nextHit(r);

		block_t* hb = map_getBlockAt(&m, r->mapX, r->mapY);
		if (hb && hb->type > 0)
		{
			hit->distance = r->distance;
			hit->hitblock = hb;
			hit->side = r->side;
			hit->hit = 1;
			hit->point = (vec2_t) {r->orig.x, r->orig.y};
			return;
		}
	}
}

void game_render(SDL_Surface* sf)
{
	graphics_clear(sf);

	vec2_t plane = {p.dir.y*0.66, -p.dir.x*0.66};
	vec2_normalize(&plane, &plane);

	for (int x=0; x<sf->w; x++)
	{
		double cam = 2*x/(double) sf->w - 1;

		ray_t r;
		r.orig = (vec2_t){p.pos.x, p.pos.y};
		r.dir = (vec2_t){p.dir.x + plane.x * cam, p.dir.y + plane.y * cam};

		hit_t hit;

		cast(sf, &r, &hit);

		if (!hit.hit)
			continue;

		double dist = hit.distance / sqrt(1+cam*cam);

		int lineh = (int)(sf->h / dist);

		if (hit.hitblock->type  == 1)
			graphics_vline(sf, x, sf->h/2 - lineh/2, sf->h/2 + lineh/2, 0x0000FF - hit.side * 0x000022);
		else if(hit.hitblock->type == 2)
			graphics_vline(sf, x, sf->h/2 - lineh/2, sf->h/2 + lineh/2, 0x00FF00 - hit.side * 0x002200);
		else
			graphics_vline(sf, x, sf->h/2 - lineh/2, sf->h/2 + lineh/2, 0xFF0000 - hit.side * 0x220000);
	}
}

void game_destroy()
{
	map_destroy(&m);
}
