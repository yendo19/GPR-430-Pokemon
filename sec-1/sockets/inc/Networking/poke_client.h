#pragma once
#include <string>

class Socket;
class myParty;
enum State;

class PokemonClient
{
private:
	Socket* connected_sock;

	char message_buffer[4096];
	myParty* party;

	int client_id;

	State current_state;

public:

	PokemonClient(const char* host, int port);
	~PokemonClient();

	void initParty();
	void update(float dt, int frame_num);
	
	myParty* getParty();

	void sendToServer(std::string data);
	std::string receivePacket();
	void processPacket(std::string msg);

	int getId() { return client_id; }
};


