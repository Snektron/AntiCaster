#ifndef RAYCAST_H_
#define RAYCAST_H_

#include "math/vec2.h"
#include "map/map.h"
#include "util/hit.h"

#define SIDE_NORTH 0
#define SIDE_EAST 1
#define SIDE_SOUTH 2
#define SIDE_WEST 3

typedef struct
{
	vec2_t dir;
	vec2_t pos;
} camera_t;

typedef struct
{
	int mapX, mapY, side;
	vec2_t startdir, dir, orig;
	double distance;
} ray_t;

void raycast_render(SDL_Surface* sf, camera_t* cam, map_t* map);
void raycast_travel(camera_t* cam, double distance, map_t* map);

#endif /* RAYCAST_H_ */
