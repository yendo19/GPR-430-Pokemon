#pragma once
#include <string>
#include "../../socklib.h"
#include "../../PokemonCreation/Pokemon.h"
#include "../../PokemonCreation/MyParty.h"

class PokemonClient
{
private:
	Socket* connected_sock;

	char message_buffer[4096];
	myParty party;

public:

	PokemonClient(const char* host, int port);
	~PokemonClient();

	void initParty();
	void update(float dt, int frame_num);
	
	myParty getParty()
	{
		return party;
	}

	void sendToServer(const char* data);
	std::string receivePacket();
};
#pragma endregion


