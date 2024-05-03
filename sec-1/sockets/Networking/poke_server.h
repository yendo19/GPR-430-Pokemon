#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <filesystem>
#include "../socklib.h"
#include "../PokemonCreation/MyParty.h"
#include <iostream>
#include <fstream>
#include <list>

class PokemonServer {

private:
	Socket* listen_sock;

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

	int run_server();
};
#pragma endregion