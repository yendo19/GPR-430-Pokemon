#pragma once
#include <string>
#include "../../socklib.h"
#include "../../PokemonCreation/Pokemon.h"
#include "../../PokemonCreation/MyParty.h"

class PokemonClient
{
private:
	Socket* connected_sock;
	Address address;

	char message_buffer[4096];
	myParty party;
	std::string to_display;
public:

	PokemonClient(const char* host, int port);
	~PokemonClient();

	void initParty();

	void start_client();
	std::string do_client(); // to be removed
	void Update(float dt, int frame_num);
	
	myParty getParty()
	{
		return party;
	}
};
#pragma endregion


