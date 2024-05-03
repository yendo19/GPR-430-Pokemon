#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void setupSDL(SDL_Window** wind, SDL_Renderer** rend, TTF_Font** encode);