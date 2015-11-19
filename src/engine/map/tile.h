#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>
#include "map.h"
#include "../math/vec2.h"
#include "../util/hit.h"
#include "../graphics/graphics.h"

#define FLAG_COLLIDABLE 0x01
#define FLAG_VISIBLE 0x02

typedef struct
{
	int x, h;
	double cx, distance;
	hit_t* hit;
	int rec;
	vec2_t* dir;
	struct map* map;
} tileRenderData_t;

typedef struct tile
{
	int flags; // flag for collision, rendertype etc;
	color_t color;
	void (*render)(SDL_Surface* sf, struct tile* t, tileRenderData_t* data);
	vec2_t space;
} tile_t;

int tile_isCollidable(tile_t* t);
int tile_isVisible(tile_t* t);

void tile_renderSolid(SDL_Surface* sf, tile_t* t, tileRenderData_t* d);
void tile_renderMirror(SDL_Surface* sf, tile_t* t, tileRenderData_t* d);

#define TILE(flags, color, renderfunc, vec) ((tile_t){flags, color, renderfunc, (vec)})
#define TILE_WALL(color) ((tile_t){FLAG_COLLIDABLE | FLAG_VISIBLE, (color), &tile_renderSolid, (vec2_t) {1., 1.}})
#define TILE_MIRROR() ((tile_t){FLAG_COLLIDABLE | FLAG_VISIBLE, 0x808080, &tile_renderMirror, (vec2_t) {1., 1.}})
#define TILE_EMPTY ((tile_t){0, 0, 0, (vec2_t) {1., 1.}})
#define TILE_SPACE(vec) ((tile_t){0, 0, 0, (vec)})

#endif /* TILE_H_ */
