#include <SDL2/SDL.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "game.h"
#include "engine/raycast.h"
#include "engine/map/map.h"
#include "engine/map/tile.h"
#include "engine/math/vec2.h"
#include "engine/util/graphics.h"

#define PLAYER_MOVESPEED 0.05
#define PLAYER_ROTSPEED 0.007

camera_t player;
map_t m;

void game_init()
{
	player.pos = (vec2_t) {5, 5};
	player.dir = (vec2_t) {-1., 0.};

	tile_t defaultWall = TILE_EMPTY;
	tile_t bluewall = TILE_WALL(1,0xFF); //{1, FLAG_COLLIDABLE, 0, (vec2_t){1., 1.}};
	tile_t greenwall = TILE_WALL(2,0xFF00);
	tile_t redwall = TILE_WALL(3,0xFF00);
	tile_t compressedWall = TILE_SPACE(((vec2_t){1., .1}));
	tile_t stretchedWall = TILE_SPACE(((vec2_t){1., 10.}));

	map_init(&m, 20, 20, &defaultWall);

	for (int i=0; i<20; i++)
	{
		map_setTileAt(&m, i, 0, &bluewall);
		map_setTileAt(&m, i, 19, &bluewall);
		map_setTileAt(&m, 0, i, &bluewall);
		map_setTileAt(&m, 19, i, &bluewall);
	}

	map_setTileAt(&m, 10, 10, &greenwall);
	map_setTileAt(&m, 11, 10, &compressedWall);
	map_setTileAt(&m, 12, 10, &compressedWall);
	map_setTileAt(&m, 13, 10, &compressedWall);
	map_setTileAt(&m, 14, 10, &greenwall);

	map_setTileAt(&m, 10, 8, &greenwall);
	map_setTileAt(&m, 11, 8, &stretchedWall);
	map_setTileAt(&m, 12, 8, &stretchedWall);
	map_setTileAt(&m, 13, 8, &stretchedWall);
	map_setTileAt(&m, 14, 8, &greenwall);

	map_setTileAt(&m, 11, 16, &redwall);
}

int game_update(int mousedx, int mousedy)
{
	const uint8_t* keys = SDL_GetKeyboardState(NULL);
	vec2_rotate(&player.dir, mousedx*PLAYER_ROTSPEED, &player.dir);

	vec2_t dir = {0, 0};
	double spd = PLAYER_MOVESPEED;

	if (keys[SDL_SCANCODE_LCTRL])
		spd *= .1;
	if (keys[SDL_SCANCODE_LSHIFT])
		spd *= 10.;

	vec2_t dirCrossed = {player.dir.y, -player.dir.x};
	if (keys[SDL_SCANCODE_W])
		vec2_add(&dir, &player.dir, &dir);
	if (keys[SDL_SCANCODE_S])
		vec2_addScale(&dir, &player.dir, -1, &dir);
	if (keys[SDL_SCANCODE_A])
		vec2_addScale(&dir, &dirCrossed, -1, &dir);
	if (keys[SDL_SCANCODE_D])
		vec2_add(&dir, &dirCrossed, &dir);

	if (vec2_lengthSq(&dir) > 0.00001)
	{
		vec2_normalize(&dir, &dir);
		camera_t cam = player;
		cam.dir = dir;
		raycast_travel(&cam, spd, &m);
		player.pos = cam.pos;
	}
	return keys[SDL_SCANCODE_ESCAPE];
}

void game_render(SDL_Surface* sf)
{
	graphics_clear(sf);

	raycast_render(sf, &player, &m);
}

void game_destroy()
{
	map_destroy(&m);
}
