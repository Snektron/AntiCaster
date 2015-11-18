#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include "raycast.h"
#include "map.h"
#include "math/vec2.h"
#include "util/graphics.h"
#include "util/util.h"

void cast(ray_t* r, hit_t* hit, map_t* map);
void raystep(ray_t* r, map_t* map);

// render a map
void raycast_render(SDL_Surface* sf, camera_t* cam, map_t* map)
{
	vec2_t plane = {cam->dir.y*0.66, -cam->dir.x*0.66};
	vec2_normalize(&plane, &plane);

	for (int x=0; x<sf->w; x++)
	{
		double cx = 2*x/(double) sf->w - 1;

		ray_t r;
		r.orig = cam->pos;
		r.startdir = (vec2_t){cam->dir.x + plane.x * cx, cam->dir.y + plane.y * cx};

		hit_t hit;

		cast(&r, &hit, map);

		if (!hit.hit)
			continue;

		double dist = hit.distance / sqrt(1+cx*cx);

		int lineh = (int)(sf->h / dist);

		if (hit.hitblock->type  == 1)
			graphics_vline(sf, x, sf->h/2 - lineh/2, sf->h/2 + lineh/2, 0x0000FF - hit.side * 0x000022);
		else if(hit.hitblock->type == 2)
			graphics_vline(sf, x, sf->h/2 - lineh/2, sf->h/2 + lineh/2, 0x00FF00 - hit.side * 0x002200);
		else
			graphics_vline(sf, x, sf->h/2 - lineh/2, sf->h/2 + lineh/2, 0xFF0000 - hit.side * 0x220000);
	}
}

// travels the camera a distance
void raycast_travel(camera_t* cam, double distance, map_t* map)
{
	ray_t r;

	r.orig = cam->pos;
	r.startdir = cam->dir;
	r.distance = 0;

	r.mapX = (int) r.orig.x;
	r.mapY = (int) r.orig.y;

	block_t* b;

	while (distance > 0)
	{
		raystep(&r, map);

		b = map_getBlockAt(map, (int) cam->pos.x,  (int) cam->pos.y);
		vec2_t s = b? b->space : (vec2_t){1,1};
		vec2_t oldpos = cam->pos;

		vec2_mul(&s, &cam->dir, &s);
		if (distance - r.distance < 0)
			vec2_addScale(&cam->pos, &s, distance, &cam->pos);
		else
			vec2_addScale(&cam->pos, &s, r.distance*1.0001, &cam->pos);
		distance -= r.distance;
		r.distance = 0;

		b = map_getBlockAt(map, (int) cam->pos.x,  (int) cam->pos.y);
		if (b && b->type > 0)
		{
			cam->pos = oldpos;
			break;
		}
	}
}

void cast(ray_t* r, hit_t* hit, map_t* map)
{
	hit->hit = 0;

	r->mapX = (int) r->orig.x;
	r->mapY = (int) r->orig.y;
	r->distance = 0;

	for (int _=0; _<100; _++)
	{
		raystep(r, map);

		block_t* hb = map_getBlockAt(map, r->mapX, r->mapY);
		if (hb && hb->type > 0)
		{
			hit->distance = r->distance;
			hit->hitblock = hb;
			hit->side = r->side;
			hit->hit = 1;
			hit->point = r->orig;
			return;
		}
	}
}

// get next block hit.
void raystep(ray_t* r, map_t* map)
{
	block_t* b = map_getBlockAt(map, r->mapX, r->mapY);
	vec2_t s = b? b->space : (vec2_t){1,1};

	r->dir = r->startdir;

	vec2_mul(&r->dir, &s, &r->dir);
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
		vec2_scale(&r->dir, sideDistX, &d);
		r->mapX += stepX;
	} else
	{
		vec2_scale(&r->dir, sideDistY, &d);
		r->mapY += stepY;
	}

	vec2_add(&r->orig, &d, &r->orig);
	vec2_div(&d, &s, &d);
	r->distance += sqrt(d.x*d.x + d.y*d.y);

	r->side = sideDistX < sideDistY ?
			stepX < 0 ? SIDE_WEST : SIDE_EAST:
			stepY < 0 ? SIDE_NORTH : SIDE_SOUTH;
}
