#pragma once
#include "string"
#include <iostream>
#include <functional>

#include "SDL.h"
#include "SDL_ttf.h"

class Button
{
public:
	Button(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Color highlightColor, std::string message);
	~Button();

	void updateCallback(std::function<void(std::string data)> func, std::string data);
	void updateRect(int width, int height, int posX, int PosY);
	void update(int x, int y, bool mbPressed);

private:
	bool mouseInBounds(int x, int y);
	std::function<void(std::string data)> callback;
	SDL_Renderer* rend;
	SDL_Color bgColor;
	SDL_Color highlightColor;
	std::string label;
	SDL_Rect rect;

	SDL_Surface* surfaceMessage;
	SDL_Texture* text;

	std::string data;
};