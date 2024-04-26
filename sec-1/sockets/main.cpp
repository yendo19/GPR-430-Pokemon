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

#include "SDL.h"

#include "button.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* wind;
SDL_Renderer* rend;

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

int setupSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	wind = SDL_CreateWindow("Pokeman Duel",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!wind)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	/* Create a renderer */
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(wind, -1, render_flags);
	if (!rend)
	{
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(wind);
		SDL_Quit();
		return 0;
	}
}

#undef main
int main()
{

	std::string filePath = getCurrentLocation();
	filePath += "/PokemonCreation";

	myParty myInv = myParty();
	myInv.Init(filePath);
	myInv.CreateFolder();
	//myInv.Update();

	myInv.updatePc();
	myInv.readPC();

	setupSDL();

	bool running = true;
	SDL_Event event;

	Button b = Button(rend, SDL_Color{255, 50, 50, 255});
	b.updateRect(50, 50, 50, 50);

	int* mouseX = 0;
	int* mouseY = 0;

	//GAME LOOP
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		SDL_GetMouseState(mouseX, mouseY);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		//b.update(*mouseX, *mouseY);
		
		SDL_RenderPresent(rend);
	}

	return 0;
}

//int main(int argc, char *argv[]) {
//
//	std::cout << "hi";
//
//	myInv.Init();

	//SockLibInit();
	//atexit(SockLibShutdown);

	//if (argc > 1) {
	//	return run_server();
	//}

	//// Initialize the system
	//float last_frame = now();
	//bool quit = false;
	//int frame_num = 0;
	//// One second per frame -- wow, that's slow!
	//const float targetDt = 1;

	//// Initialize the player "object"
	//float character_pos = 0;
	//float xVelocity = 1;

	//// Initialize the socket system
	//NetworkModule netsys;
	//netsys.Init();

	//GameObject go;
	//go.x = 100001;
	//go.y = 100002;
	//go.z = 100003;
	//go.xVel = 100010;
	//go.yVel = 100011;
	//go.zVel = 100012;

	//go.sprite = "Hello, there!";
	//std::cout << "===== Game Object as Bytes =====\n";
	//print_as_bytes((char*)&go, sizeof(go));

	//{
	//	char buffer[4096];
	//	size_t amt_written = SerializeGameObjectAsString(&go, buffer, sizeof(buffer));
	//	std::string go_str(buffer, amt_written);
	//	std::cout << "===== Game Object as String =====\n";
	//	std::cout << "===== Size: " << amt_written << "   ====\n";
	//	std::cout << "\"" << go_str << "\"" << std::endl;
	//}

	//{
	//	char buffer[4096] = { 0 };
	//	size_t amt_written = SerializeGameObjectAsBytes(&go, buffer, sizeof(buffer));
	//	std::cout << "==== Serialized game object as Bytes =====\n";
	//	std::cout << "===== Size: " << amt_written << "   ====\n";
	//	print_as_bytes(buffer, amt_written);
	//}

	//return 0;

	//while (!quit) {
	//	float time = now();
	//	float dt = time - last_frame;
	//	if (dt < targetDt)
	//		continue;
	//	frame_num++;

	//	// Process input -- is the keyboard pressed,
	//	// is the mouse down, etc.?
	//	netsys.Update(dt, frame_num);
	//	
	//	// Update all the game objects
	//	character_pos += xVelocity * dt;

	//	// Render
	//	// Clear the back buffer
	//	system("cls");

	//	for (int i = 0; i < character_pos; i++) {
	//		std::cout << " ";
	//	}
	//	std::cout << "@";
	//	std::cout << std::endl << std::endl;
	//	std::cout << to_display;

	//	to_display = "";
	//	last_frame = time;
	//}


	//return 0;
//
