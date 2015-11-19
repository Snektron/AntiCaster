#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include "raycast.h"
#include "graphics/graphics.h"
#include "map/map.h"
#include "map/tile.h"
#include "math/vec2.h"
#include "util/util.h"

#define MAX_RECURSION 2

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
		r.distance = 0;

		raycast_column(sf, &r, map, x, cx, 0);
	}
}

void raycast_column(SDL_Surface* sf, ray_t* r, map_t* map, int x, double cx, int rec)
{
	hit_t hit;
	cast(r, &hit, map);
	if (!hit.hit)
		return;

	double dist = hit.distance / sqrt(1+cx*cx);
	int lineh = (int)(sf->h / dist);
	tile_t* tile = map_getTileAt(map, r->mapX, r->mapY);
	if (tile && tile_isVisible(tile) && tile->render && rec < MAX_RECURSION)
	{
		tileRenderData_t rd;
		rd.x = x;
		rd.h = lineh;
		rd.cx = cx;
		rd.hit = &hit;
		rd.rec = rec+1;
		rd.dir = &r->startdir;
		rd.map = map;
		rd.distance = r->distance;
		tile->render(sf, tile, &rd);
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

	tile_t* t;

	while (distance > 0)
	{
		raystep(&r, map);

		t = map_getTileAt(map, (int) cam->pos.x,  (int) cam->pos.y);
		vec2_t s = t? t->space : (vec2_t){1,1};
		vec2_t oldpos = cam->pos;

		vec2_mul(&s, &cam->dir, &s);
		if (distance - r.distance < 0)
			vec2_addScale(&cam->pos, &s, distance, &cam->pos);
		else
			vec2_addScale(&cam->pos, &s, r.distance*1.0001, &cam->pos);
		distance -= r.distance;
		r.distance = 0;

		t = map_getTileAt(map, (int) cam->pos.x,  (int) cam->pos.y);
		if (t && tile_isCollidable(t))
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

	for (int _=0; _<100; _++)
	{
		raystep(r, map);

		tile_t* ht = map_getTileAt(map, r->mapX, r->mapY);
		if (ht && tile_isVisible(ht))
		{
			hit->distance = r->distance;
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
	tile_t* t = map_getTileAt(map, r->mapX, r->mapY);
	vec2_t s = t? t->space : (vec2_t){1,1};

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
			stepX < 0 ? SIDE_EAST : SIDE_WEST:
			stepY < 0 ? SIDE_NORTH : SIDE_SOUTH;
}

void raycast_getSideNormal(int side, vec2_t* out)
{
	switch(side)
	{
		case SIDE_NORTH:
			out->x = 0;
			out->y = 1;
			break;
		case SIDE_EAST:
			out->x = 1;
			out->y = 0;
			break;
		case SIDE_SOUTH:
			out->x = 0;
			out->y = -1;
			break;
		case SIDE_WEST:
			out->x = -1;
			out->y = 0;
			break;
	}
}
