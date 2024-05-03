#include <iostream>
#include "ui.h"

void UiManager::setup()
{
	bool running = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	wind = SDL_CreateWindow("Pokeman(?) Duel",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (wind == NULL)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}

	/* Create a renderer */
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(wind, -1, render_flags);
	if (rend == NULL)
	{
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(wind);
		SDL_Quit();
	}

	if (TTF_Init() != 0)
	{
		printf("Error Initializing TTF%s\n", TTF_GetError());
		TTF_Quit();
	}

	encode = TTF_OpenFont("assets/EncodeRegular.ttf", 12);
	if (encode == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}