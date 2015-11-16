#ifndef MAP_H_
#define MAP_H_

typedef struct
{
	int type;
	double spacex, spacey;
} block_t;

typedef struct
{
	int width, height;
	block_t* map;
} map_t;

void map_init(map_t* map, int width, int height, block_t* defaultBlock);
void map_destroy(map_t* map);
void map_setBlockAt(map_t* map, int x, int y, block_t* block);
block_t* map_getBlockAt(map_t* map, int x, int y);

#endif /* MAP_H_ */
