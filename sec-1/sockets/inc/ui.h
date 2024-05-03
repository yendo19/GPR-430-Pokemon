#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "button.h"
#include "../PokemonCreation/Pokemon.h"
#include <time.h>
#include <iostream>
#include "GameManager.h"
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int NUM_SPRITES = 9;

class UiManager {
private:
	int* mouseX;
	int* mouseY;
	SDL_Event event;
	bool running;

	SDL_Window* wind;
	SDL_Renderer* rend;
	TTF_Font* encode;

	std::vector<SDL_Texture*> sprites;

public:
	UiManager()
	{
		mouseX = new int(0);
		mouseY = new int(0);

		setup();
	}

	~UiManager()
	{
		delete mouseX;
		delete mouseY;
	}

	void setup();

	void setupActive(std::list<Button>* attacks, Pokemon active);

	void initSprites();

	bool update(std::list<Button>* attacks, float dt);

	void updateTeam(Player p, bool allied, float dt);

};