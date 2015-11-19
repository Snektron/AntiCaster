#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>
#include "../util/util.h"

typedef uint32_t color_t;

#define COLORD(r, g, b) ((((int) ((r)*0xFF) & 0xFF) << 16) | ((int) (((g)*0xFF) & 0xFF) << 8) | ((int) ((b)*0xFF) & 0xFF))
#define COLORI(r, g, b) ((((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))
#define RED(col) ((col) >> 16 & 0xFF)
#define GREEN(col) ((col) >> 8 & 0xFF)
#define BLUE(col) ((col) & 0xFF)

color_t color_mul(color_t c0, color_t c1);
color_t color_scale(color_t c0, int x);
color_t color_add(color_t c0, color_t c1);
color_t color_sub(color_t c0, color_t c1);

#endif /* COLOR_H_ */
