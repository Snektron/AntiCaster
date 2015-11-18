#include <malloc.h>
#include "map.h"
#include "math/vec2.h"

void map_init(map_t* map, int width, int height, block_t* defaultBlock)
{
	map->width = width;
	map->height = height;
	map->map = (block_t*) malloc(width*height*sizeof(block_t));

	if (map->map)
		for (int i=0; i<width*height; i++)
			map->map[i] = *defaultBlock;
}

void map_destroy(map_t* map)
{
	if (map->map)
	{
		free(map->map);
		map->map = 0;
	}
}

void map_setBlockAt(map_t* map, int x, int y, block_t* block)
{
	if (x >= 0 && x < map->width && y >= 0 && y < map->height)
		map->map[y*map->width+x] = *block;
}

block_t* map_getBlockAt(map_t* map, int x, int y)
{
	if (x >= 0 && x < map->width && y >= 0 && y < map->height)
		return &map->map[y*map->width+x];
	return 0;
}


block_t* map_getBlockAtVec(map_t* map, vec2_t* pos)
{
	return map_getBlockAt(map, (int) pos->x, (int) pos->y);
}
