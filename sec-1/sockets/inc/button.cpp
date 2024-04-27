#include "button.h"
#include <iostream>


Button::Button(SDL_Renderer* rend,TTF_Font* font, SDL_Color color, SDL_Color highlightColor, std::string message)
{
	this->rend = rend;
	this->bgColor = color;
	this->highlightColor = highlightColor;

	surfaceMessage = TTF_RenderText_Solid(font, message.data(), SDL_Color{255, 255, 255});
	text = SDL_CreateTextureFromSurface(rend, surfaceMessage);
}

Button::~Button()
{
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(text);
}

void Button::updateCallback(std::function<void(std::string data)> func, std::string data)
{
	callback = func;
	this->data = data;
}

void Button::updateRect(int width, int height, int posX, int posY)
{
	rect.w = width;
	rect.h = height;
	rect.x = posX;
	rect.y = posY;
}

bool Button::mouseInBounds(int x, int y)
{
	// Too far left
	if (x < rect.x) return false;

	// Too far right
	if (x > rect.x + rect.w) return false;

	// Too high
	if (y < rect.y) return false;

	// Too low
	if (y > rect.y + rect.h) return false;

	// Inside rectangle
	return true;
}

void Button::update(int x, int y, bool mbPressed)
{
	bool inBounds = mouseInBounds(x, y);
	SDL_Color c = ( inBounds ? highlightColor : bgColor);
	SDL_SetRenderDrawColor(rend, c.r, c.g, c.b, c.a);
	if (SDL_RenderDrawRect(rend, &rect) != 0)
	{

		std::cout << "ERROR: Cannot draw button " << SDL_GetError() << std::endl;
	}

	SDL_RenderCopy(rend, text, NULL, &rect);

	if (inBounds && mbPressed)
	{
		this->callback(data);
	}
}
