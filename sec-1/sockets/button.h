#pragma once
#include "string"
#include <stdio.h>
#include "SDL.h"

class Button
{
public:
	Button(SDL_Renderer* renderer, SDL_Color color);
	~Button();

	void updateRect(int width, int height, int posX, int PosY);
	void update(int x, int y);
	bool mouseInBounds(int x, int y);

private:
	SDL_Renderer* renderer;
	SDL_Color bgColor;
	std::string label;
	SDL_Rect rect;
};