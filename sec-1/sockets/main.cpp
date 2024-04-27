#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <filesystem>
#include "socklib.h"
#include "defer.h"
#include "PokemonCreation/MyParty.h"
#include <iostream>
#include <fstream>
#include <list>

#include "SDL.h"
#include "SDL_ttf.h"

#include "inc/button.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* wind;
SDL_Renderer* rend;
TTF_Font* encode;

int run_server() {
	// Simple demo to demonstrate serialization
	// over TCP
	// Create a socket, wait for folks to connect
	Socket listen_sock(Socket::Family::INET, Socket::Type::STREAM);
	listen_sock.Bind(Address("0.0.0.0", 36925));
	listen_sock.Listen();

	while (true) {
		std::cout << "server Running";
	}
}

std::string getCurrentLocation() {
	std::filesystem::path main_cpp_path = __FILE__;
	std::filesystem::path main_cpp_folder = main_cpp_path.parent_path();
	return main_cpp_folder.string();
}

void setupSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	wind = SDL_CreateWindow("Pokeman(?) Duel",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!wind)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}

	/* Create a renderer */
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(wind, -1, render_flags);
	if (!rend)
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
	if (encode == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}

//FROM IN CLASS CODE
float clocks_to_secs(clock_t clocks) {
	return (float)clocks / CLOCKS_PER_SEC;
}

//FROM IN CLASS CODE
float time_now() {
	return clocks_to_secs(clock());
}

void pickAttack(std::string data)
{
	//TODO:send the chosen attack's power to the other player
	//for now, just print out the data
	std::cout << data;
}

std::string serializeMove(Pokemon pkmn, attacks atk)
{
	std::string temp;
	std::stringstream num;
	std::string numbers;
	
	num.str("");
	num << pkmn.getSpeed();
	numbers = num.str();
	for (char c : numbers)
		temp.push_back(c);
	temp.push_back(':');
	for (char c : atk.getName())
		temp.push_back(c);
	temp.push_back(':');
	num.str("");
	num.clear();
	numbers.erase();
	num << atk.getDamage();
	numbers = num.str();
	for (char c : numbers)
		temp.push_back(c);

	return temp;
}

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

		std::string move = serializeMove(active, active.getAttackAt(i));

		attacks->back().updateCallback(pickAttack, move);
		attacks->back().updateRect(100, 50, buttonPosX, buttonPosY);
		buttonPosX += buttonSpacingX;
		if ((i + 1) % 2 == 0)
		{
			buttonPosX = buttonPosXStart;
			buttonPosY += buttonSpacingY;
		}
	}
}

#undef main
int main()
{
	std::string filePath = getCurrentLocation();
	filePath += "\\PokemonCreation";
	//std::cout << filePath;
	std::list<Pokemon> temp;
	myParty myInv = myParty();
	myInv.Init(filePath);
	myInv.CreateFolder();
	myInv.updatePc();
	


	myInv.Update();

	system("pause");



	setupSDL();

	bool running = true;
	SDL_Event event;

	std::list<Button> attacks;

	setupAttacks(&attacks, myInv.getPokemonInPartyAt(0));

	int* mouseX = new int(0);
	int* mouseY = new int(0);
	float last_frame_time = time_now();
	float targetDt = 1 / 60.0f;

	//GAME LOOP
	while (running)
	{
		float now = time_now();
		float dt = now - last_frame_time;
		if (dt < targetDt)
			continue;

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
	}

	delete mouseX;
	delete mouseY;

	return 0;
}