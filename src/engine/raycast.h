#ifndef RAYCAST_H_
#define RAYCAST_H_

#include "math/vec2.h"
#include "map/map.h"
#include "util/hit.h"

#define SIDE_NORTH 0
#define SIDE_EAST 1
#define SIDE_SOUTH 2
#define SIDE_WEST 3

#define SIDE_OPPOSITE(side) (((side)+2)%4)
#define SIDE_RIGHT(side) (((side)+1)%4)
#define SIDE_LEFT(side) (((side)+3)%4)

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
void raycast_column(SDL_Surface* sf, ray_t* r, map_t* map, int x, double cx, int rec);
void raycast_travel(camera_t* cam, double distance, map_t* map);
void raycast_getSideNormal(int side, vec2_t* out);

#endif /* RAYCAST_H_ */
