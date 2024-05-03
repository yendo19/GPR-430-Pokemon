#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "../../socklib.h"
#include <iostream>
#include <fstream>
#include <list>

enum State;

class PokemonServer {

private:
	int connections;
	Socket* listen_sock;
	std::vector<Socket*> connection_sockets;

	State state;

public:
	/// <summary>
	/// This function accepts a hostname/IP and port number, 
	/// creates a TCP socket for the server to use, 
	/// and binds this socket to the provided host and port ("localhost" on port 20000).
	/// </summary>
	/// <param name="host"></param>
	/// <param name="port"></param>
	PokemonServer(const char* host, int port);

	~PokemonServer();

	void update(float dt, int frame_num);
	void acceptConnections();
	void acceptConnection();

	void sendToClient(int client_id, std::string data);
	void sendToAllClients(std::string data);

	void processPacket(std::string msg);

	void setState(State state) { this->state = state; }
	State getState() { return this->state; }
};
#pragma endregion