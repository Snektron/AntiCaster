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

double vec2_lengthSq(vec2_t* v)
{
	return v->x*v->x + v->y*v->y;
}

double vec2_length(vec2_t* v)
{
	return sqrt(vec2_lengthSq(v));
}

void vec2_normalize(vec2_t* v0, vec2_t* out)
{
	double length = vec2_length(v0);
	if (length < 0.000001)
		return;
	out->x = v0->x/length;
	out->y = v0->y/length;
}

void vec2_rotate(vec2_t* v0, double angle, vec2_t* out)
{
	double v0x = v0->x;
	out->x = v0->x * cos(-angle) - v0->y * sin(-angle);
	out->y = v0x * sin(-angle) +  v0->y * cos(-angle);
}

void vec2_mul(vec2_t* v0, vec2_t* v1, vec2_t* out)
{
	out->x = v0->x * v1->x;
	out->y = v0->y * v1->y;
}

void vec2_div(vec2_t* v0, vec2_t* v1, vec2_t* out)
{
	out->x = v0->x / v1->x;
	out->y = v0->y / v1->y;
}

void vec2_scale(vec2_t* v0, double scale, vec2_t* out)
{
	out->x = v0->x * scale;
	out->y = v0->y * scale;
}

double vec2_dot(vec2_t* v0, vec2_t* v1)
{
	return v0->x * v0->y + v1->x * v1->y;
}

void vec2_reflect(vec2_t* i, vec2_t* n, vec2_t* out)
{
	vec2_scale(n, 2*vec2_dot(i, n), out);
	vec2_addScale(i, out, -1, out);
}
