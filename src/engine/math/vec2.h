#ifndef VEC2_H_
#define VEC2_H_

typedef struct
{
	double x, y;
} vec2_t;

void vec2_add(vec2_t* v0, vec2_t* v1, vec2_t* out);
void vec2_addScale(vec2_t* v0, vec2_t* v1, double scale, vec2_t* out);
double vec2_lengthSq(vec2_t* v);
double vec2_length(vec2_t* v);
void vec2_normalize(vec2_t* v0, vec2_t* out);
void vec2_rotate(vec2_t* v0, double angle, vec2_t* out);
void vec2_mul(vec2_t* v0, vec2_t* v1, vec2_t* out);
void vec2_div(vec2_t* v0, vec2_t* v1, vec2_t* out);
void vec2_scale(vec2_t* v0, double scale, vec2_t* out);
double vec2_dot(vec2_t* v0, vec2_t* v1);
void vec2_reflect(vec2_t* i, int side, vec2_t* out);

#endif /* VEC2_H_ */
