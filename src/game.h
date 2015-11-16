
#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

void game_init();
int game_update(int mousedx, int mousedy);
void game_render(SDL_Surface* sf);
void game_destroy();

#endif /* GAME_H_ */
