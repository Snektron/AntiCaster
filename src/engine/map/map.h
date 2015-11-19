#ifndef MAP_H_
#define MAP_H_

#include "../math/vec2.h"
#include "tile.h"

typedef struct map
{
	int width, height;
	tile_t def;
	tile_t* map;
} map_t;

void map_init(map_t* map, int width, int height, tile_t* defaultTile);
void map_destroy(map_t* map);
void map_setTileAt(map_t* map, int x, int y, tile_t* tile);
tile_t* map_getTileAt(map_t* map, int x, int y);

#endif /* MAP_H_ */
