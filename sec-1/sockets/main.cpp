#include <iostream>
#include <sstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <random>

#include <stdlib.h>
#include <filesystem>

#include <iostream>
#include <fstream>
#include <list>

#include "inc/ui.h"

#include "inc/Networking/poke_server.h"
#include "inc/Networking/poke_client.h"
#include "inc/GameManager.h"

float ticks_to_sec(clock_t ticks) {
	return (float)ticks / CLOCKS_PER_SEC;
}

float now() {
	return ticks_to_sec(clock());
}

#undef main
int main(int argc, char* argv[])
{
	SockLibInit();
	atexit(SockLibShutdown);

	GameManager gm = GameManager::GetGameManager();
	PokemonServer* server = nullptr;

	if (argc > 1) {
		// HOST ===========================
		// create the server
		server = new PokemonServer("127.0.0.1", 69420);
		gm.setServer(server);
	}

	// create the client to connect to the server
	PokemonClient client = PokemonClient("127.0.0.1", 69420);
	gm.setClient(&client);

	if(server != nullptr)
		server->acceptConnections();

	// Initialize the system
	float last_frame = now();
	bool quit = false;
	int frame_num = 0;
	// One second per frame -- wow, that's slow!
	const float targetDt = 1 / 60.0f;

	// initialize UI
	UiManager ui = UiManager();

	ui.initSprites();

	std::vector<int> spriteIndexes(16);
	std::iota(std::begin(spriteIndexes), std::end(spriteIndexes), 0);

	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(spriteIndexes), std::end(spriteIndexes), rng);

	if (gm.getIsServer())
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				gm.getPlayerAtIndex(i)->party[j].setSprite(i);
			}
		}
	}

	// init attack UI
	std::list<Button> attacks;
	ui.setupActive(&attacks, client.getParty().getPokemonInPartyAt(0));


	//GAME LOOP
	bool running = true;
	while (running)
	{
		// update frame number
		float time = now();
		float dt = time - last_frame;
		if (dt < targetDt)
			continue;
		frame_num++;

		// update UI every frame
		running = ui.update(&attacks, dt); // will return false if player gives signal to quit


		// only update client and server every other frame
		if (frame_num % 60 != 0) continue;


		std::cout << "==========================\n";
		std::cout << "Frame " << frame_num << "\n";

		gm.update(dt, frame_num);
	}

	return 0;
}