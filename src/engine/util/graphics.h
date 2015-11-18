#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL2/SDL.h>

typedef uint32_t color_t;

void graphics_clear(SDL_Surface* sf);
void graphics_setPixel(SDL_Surface* sf, int x, int y, color_t c);
void graphics_vline(SDL_Surface* sf, int x, int y0, int y1, color_t c);
void graphics_hline(SDL_Surface* sf, int y, int x0, int x1, color_t c);
void graphics_line(SDL_Surface* sf, int x0, int x1, int y0, int y1, color_t c);

#endif /* GRAPHICS_H_ */
