#pragma once
#include "string"
#include <stdio.h>
#include "SDL.h"
class Button
{
public:
	Button(SDL_Surface* renderer, SDL_Color color, SDL_Color highlightColor);
	~Button();

	void updateRect(int width, int height, int posX, int PosY);
	void update(int x, int y, bool mbPressed);

private:
	bool mouseInBounds(int x, int y);
	virtual void onClick();

	SDL_Surface* surface;
	SDL_Color bgColor;
	SDL_Color highlightColor;
	std::string label;
	SDL_Rect rect;
};

class AttackButton : public Button
{
private:
	void onClick() override;
};