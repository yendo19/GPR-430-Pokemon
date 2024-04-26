#include "button.h"

Button::Button(SDL_Renderer* renderer, SDL_Color color)
{
	this->renderer = renderer;
}

Button::~Button()
{

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

void Button::update(int x, int y)
{
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(renderer, &rect);
}
