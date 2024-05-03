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

	void setupAttacks(std::list<Button>* attacks, Pokemon active)
	{

		int buttonPosXStart = 100;
		int buttonPosX = buttonPosXStart;
		int buttonPosY = 100;
		int buttonSpacingX = 120;
		int buttonSpacingY = 70;

		for (int i = 0; i < 4; i++)
		{
			std::string display = active.getAttackAt(i).getName() + " : " + std::to_string(active.getAttackAt(i).getDamage());
			attacks->push_back(Button(rend, encode, SDL_Color{ 255, 50, 50, 255 }, SDL_Color{ 255, 100, 100, 255 }, display));

			std::string move = Pokemon::serializeMove(active, active.getAttackAt(i));

			attacks->back().updateCallback(Pokemon::pickAttack, move);
			attacks->back().updateRect(100, 50, buttonPosX, buttonPosY);
			buttonPosX += buttonSpacingX;
			if ((i + 1) % 2 == 0)
			{
				buttonPosX = buttonPosXStart;
				buttonPosY += buttonSpacingY;
			}
		}
	}

	bool update(std::list<Button> attacks)
	{
		SDL_GetMouseState(mouseX, mouseY);
		bool mbUp = false;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					mbUp = true;
				}
				break;
			}
		}
		SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
		SDL_RenderClear(rend);

		//UPDATE ALL OBJECTS
		for (Button b : attacks)
		{
			b.update(*mouseX, *mouseY, mbUp);
		}

		SDL_RenderPresent(rend);

		return running;
	}
};

#pragma endregion