#pragma once
#include <string>


class Socket;
class myParty;
enum State;

//#include "../../inc/GameManager.h"
//#include "../../socklib.h"
//#include "../../PokemonCreation/Pokemon.h"
//#include "../../PokemonCreation/MyParty.h"


class PokemonClient
{
private:
	Socket* connected_sock;

	char message_buffer[4096];
	myParty* party;

	int client_id;

	State current_state;
	myParty party;
	int client_id;

	static std::vector<std::string> PokemonClient::split(const std::string& s, char delim);

public:

	PokemonClient(const char* host, int port);
	~PokemonClient();

	void initParty();
	void update(float dt, int frame_num);
	
	myParty getParty();

	void sendToServer(const char* data);
	std::string receivePacket();
	void processPacket(std::string msg);
};
#pragma endregion


