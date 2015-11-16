#include <SDL2/SDL.h>
#include <stdio.h>
#include "game.h"

#define TPS 60
#define MAX_FRAMESKIP 5

int main(int argc, char* argv[])
{
	SDL_Window* window;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("AntiCaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	int quit = 0;
	SDL_Event e;
	SDL_Surface* sf = SDL_GetWindowSurface(window);

	int pressed = 0;

	game_init();

	int tpsGameTick = SDL_GetTicks(), tpsLoops;

	while (!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if( e.type == SDL_QUIT )
				quit = 1;
		}

		tpsLoops = 0;

		while(SDL_GetTicks() > tpsGameTick && tpsLoops < MAX_FRAMESKIP)
		{
			SDL_PumpEvents();
			int dmx = 0, dmy = 0, state = SDL_GetMouseState(&dmx, &dmy);
			dmx -= sf->w/2;
			dmy -= sf->h/2;
			if (!pressed)
			{
				if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					pressed = 1;
					SDL_SetWindowGrab(window, SDL_TRUE);
					SDL_ShowCursor(0);
					SDL_WarpMouseInWindow(window, sf->w/2, sf->h/2);
				}
				dmx = 0;
				dmy = 0;
			}else if (!(state & SDL_BUTTON(SDL_BUTTON_LEFT)))
			{
				SDL_ShowCursor(1);
				SDL_SetWindowGrab(window, SDL_FALSE);
				SDL_FreeSurface(sf);
				pressed = 0;
			}

			if (game_update(dmx, dmy))
				quit = 1;
			if (pressed)
				SDL_WarpMouseInWindow(window, sf->w/2, sf->h/2);
			tpsGameTick += 1000/TPS;
			tpsLoops++;
		}

		game_render(sf);
		SDL_UpdateWindowSurface(window);
	}

	game_destroy();
	SDL_ShowCursor(1);
	SDL_SetWindowGrab(window, SDL_FALSE);
	SDL_FreeSurface(sf);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
