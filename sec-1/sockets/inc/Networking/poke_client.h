#pragma once
#include <string>

extern class Socket;
extern class myParty;
extern enum State;

class PokemonClient
{
private:
	Socket* connected_sock;

	char message_buffer[4096];
	myParty* party;

	int client_id;

	State current_state;
	myParty* party;
	int client_id;

public:

	PokemonClient(const char* host, int port);
	~PokemonClient();

	void initParty();
	void update(float dt, int frame_num);
	
	myParty* getParty();

	void sendToServer(const char* data);
	std::string receivePacket();
	void processPacket(std::string msg);
};
#pragma endregion


