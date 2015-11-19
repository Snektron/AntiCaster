#include <stdlib.h>
#include "map.h"
#include "../math/vec2.h"

void map_init(map_t* map, int width, int height, tile_t* defaultTile)
{
	map->width = width;
	map->height = height;
	map->map = (tile_t*) malloc(width*height*sizeof(tile_t));
	map->def = *defaultTile;
	if (map->map)
		for (int i=0; i<width*height; i++)
			map->map[i] = *defaultTile;
}

void map_destroy(map_t* map)
{
	if (map->map)
	{
		free(map->map);
		map->map = 0;
	}
}

void map_setTileAt(map_t* map, int x, int y, tile_t* tile)
{
	if (x >= 0 && x < map->width && y >= 0 && y < map->height)
		map->map[y*map->width+x] = *tile;
}

tile_t* map_getTileAt(map_t* map, int x, int y)
{
	if (x >= 0 && x < map->width && y >= 0 && y < map->height)
		return &map->map[y*map->width+x];
	return &map->def;
}
