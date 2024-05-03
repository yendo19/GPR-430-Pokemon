#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "../inc/button.h"
#include "../PokemonCreation/Pokemon.h"
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class UiManager {
private:
	int* mouseX;
	int* mouseY;
	SDL_Event event;
	bool running;

	SDL_Window* wind;
	SDL_Renderer* rend;
	TTF_Font* encode;

public:
	UiManager()
	{
		mouseX = new int(0);
		mouseY = new int(0);
	}

	~UiManager()
	{
		delete mouseX;
		delete mouseY;
	}

	void setup();

	void setupAttacks(std::list<Button>* attacks, Pokemon active);

	bool update(std::list<Button>* attacks);
};

#pragma endregion