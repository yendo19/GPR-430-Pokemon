#pragma once
#include "string"
#include <iostream>
#include <functional>

#include "SDL.h"
#include "SDL_ttf.h"
#include "GameManager.h"
#include "Extras.h"

class Button
{
public:
	Button(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Color highlightColor, std::string message);
	~Button();

	void updateCallback(std::function<void(BattleEvent data)> func, BattleEvent data);
	void updateRect(int width, int height, int posX, int PosY);
	void update(int x, int y, bool mbPressed);

private:
	bool mouseInBounds(int x, int y);
	std::function<void(BattleEvent data)> callback;
	SDL_Renderer* rend;
	SDL_Color bgColor;
	SDL_Color highlightColor;
	std::string label;
	SDL_Rect rect;

	SDL_Surface* surfaceMessage;
	SDL_Texture* text;

    BattleEvent data;
};