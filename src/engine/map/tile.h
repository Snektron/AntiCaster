#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>
#include "../math/vec2.h"
#include "../util/hit.h"
#include "../util/graphics.h"

#define FLAG_COLLIDABLE 0x01
#define FLAG_VISIBLE 0x02

typedef struct tile
{
	int type;
	int flags; // flag for collision, rendertype etc;
	color_t color;
	void (*render)(SDL_Surface* sf, struct tile* t, int x, int down, int up, hit_t* hit);
	vec2_t space;
} tile_t;

int tile_isCollidable(tile_t* t);
int tile_isVisible(tile_t* t);
void tile_renderSolid(SDL_Surface* sf, tile_t* t, int x, int down, int up, hit_t* hit);

#define TILE_WALL(type, color) ((tile_t){type, FLAG_COLLIDABLE | FLAG_VISIBLE, (color), &tile_renderSolid, (vec2_t) {1., 1.}})
#define TILE_EMPTY ((tile_t){0, 0, 0, 0, (vec2_t) {1., 1.}})
#define TILE_SPACE(vec) ((tile_t){0, 0, 0, 0, (vec)})

#endif /* TILE_H_ */
