#include <math.h>
#include "vec2.h"

void vec2_add(vec2_t* v0, vec2_t* v1, vec2_t* out)
{
	out->x = v0->x + v1->x;
	out->y = v0->y + v1->y;
}


void vec2_addScale(vec2_t* v0, vec2_t* v1, double scale, vec2_t* out)
{
	out->x = v0->x + v1->x*scale;
	out->y = v0->y + v1->y*scale;
}

void vec2_normalize(vec2_t* v0, vec2_t* out)
{
	double length = sqrt(v0->x*v0->x + v0->y*v0->y);
	out->x = v0->x/length;
	out->y = v0->y/length;
}

void vec2_rotate(vec2_t* v0, double angle, vec2_t* out)
{
	double v0x = v0->x;
	out->x = v0->x * cos(-angle) - v0->y * sin(-angle);
	out->y = v0x * sin(-angle) +  v0->y * cos(-angle);
}
