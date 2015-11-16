#ifndef VEC2_H_
#define VEC2_H_

typedef struct
{
	double x, y;
} vec2_t;

void vec2_add(vec2_t* v0, vec2_t* v1, vec2_t* out);
void vec2_addScale(vec2_t* v0, vec2_t* v1, double scale, vec2_t* out);
void vec2_normalize(vec2_t* v0, vec2_t* out);
void vec2_rotate(vec2_t* v0, double angle, vec2_t* out);

#endif /* VEC2_H_ */
