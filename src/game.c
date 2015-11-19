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

int game_init()
{
	player.pos = (vec2_t) {5, 5};
	player.dir = (vec2_t) {-1., 0.};

	FILE *fp = fopen("map.txt", "r");
	if (!fp)
		return 1;

	tile_t defaultWall = TILE_EMPTY;
	tile_t bluewall = TILE_WALL(1,0xFF); //{1, FLAG_COLLIDABLE, 0, (vec2_t){1., 1.}};
	tile_t greenwall = TILE_WALL(2,0xFF00);
	tile_t redwall = TILE_WALL(3,0xFF00);
	tile_t compressedWall = TILE_SPACE(((vec2_t){1., .1}));
	tile_t stretchedWall = TILE_SPACE(((vec2_t){1., 10.}));
	tile_t shwall = TILE_SPACE(((vec2_t){1., .1}));
	tile_t svwall = TILE_SPACE(((vec2_t){.1, 1.}));
	tile_t chwall = TILE_SPACE(((vec2_t){1., 10.}));
	tile_t cvwall = TILE_SPACE(((vec2_t){10., 1.}));

	map_init(&m, 20, 20, &defaultWall);

	int i = 0;
	char c;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n') continue;
		else if (c == '#') map_setTileAt(&m, i/20, i%20, &bluewall);
		else if (c == 'r') map_setTileAt(&m, i/20, i%20, &redwall);
		else if (c == 'g') map_setTileAt(&m, i/20, i%20, &greenwall);
		else if (c == '|') map_setTileAt(&m, i/20, i%20, &svwall);
		else if (c == '-') map_setTileAt(&m, i/20, i%20, &shwall);
		else if (c == '.') map_setTileAt(&m, i/20, i%20, &cvwall);
		else if (c == ',') map_setTileAt(&m, i/20, i%20, &chwall);
		i++;
	}

	fclose(fp);
	return 0;

}

int game_update(int mousedx, int mousedy)
{
	const uint8_t* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_LEFT])
		mousedx -= 2;
	if (keys[SDL_SCANCODE_RIGHT])
		mousedx += 2;
	vec2_rotate(&player.dir, mousedx*PLAYER_ROTSPEED, &player.dir);

	vec2_t dir = {0, 0};
	double spd = PLAYER_MOVESPEED;

	if (keys[SDL_SCANCODE_LCTRL])
		spd *= .1;
	if (keys[SDL_SCANCODE_LSHIFT])
		spd *= 10.;

	vec2_t dirCrossed = {player.dir.y, -player.dir.x};
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
		vec2_add(&dir, &player.dir, &dir);
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
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
