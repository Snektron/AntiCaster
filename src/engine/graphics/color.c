#include "color.h"

int clamp(int x);

int clamp(int x)
{
	return x < 0 ? 0 : x > 255 ? 255 : x;
}

color_t color_mul(color_t c0, color_t c1)
{
	return COLORI(clamp(RED(c0) * RED(c1)), clamp(GREEN(c0) * GREEN(c1)), clamp(BLUE(c0) * BLUE(c1)));
}

color_t color_scale(color_t c0, int x)
{
	return COLORI(clamp(RED(c0) * x), clamp(GREEN(c0) * x), clamp(BLUE(c0) * x));
}

color_t color_add(color_t c0, color_t c1)
{
	return COLORI(clamp(RED(c0) + RED(c1)), clamp(GREEN(c0) + GREEN(c1)), clamp(BLUE(c0) + BLUE(c1)));
}

color_t color_sub(color_t c0, color_t c1)
{
	return COLORI(clamp(RED(c0) - RED(c1)), clamp(GREEN(c0) - GREEN(c1)), clamp(BLUE(c0) - BLUE(c1)));
}
