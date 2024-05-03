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

void UiManager::setupAttacks(std::list<Button>* attacks, Pokemon active)
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
bool UiManager::update(std::list<Button>* attacks)
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
	for (Button b : *attacks)
	{
		b.update(*mouseX, *mouseY, mbUp);
	}

	SDL_RenderPresent(rend);

	return running;
}