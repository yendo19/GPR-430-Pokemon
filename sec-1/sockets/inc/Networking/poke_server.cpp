#include "poke_server.h"

PokemonServer::PokemonServer(const char* host, int port)
{
	// TCP Server: Must pick STREAM for Type
	listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

	// Next: Bind to an address
	Address srv_addr(host, port);
	listen_sock->Bind(srv_addr);
	listen_sock->Listen();
}

PokemonServer::~PokemonServer()
{
	delete listen_sock;
	listen_sock = NULL;
}

int PokemonServer::run_server() {
	// Simple demo to demonstrate serialization
	// over TCP
	// Create a socket, wait for folks to connect

	while (true) {
		Socket conn_sock = listen_sock->Accept();
		bool connection_alive = true;
		std::vector<Pokemon*> game_objects;
		while (connection_alive) {
			char buffer[4096];

			int nbytes_recvd = conn_sock.Recv(buffer, sizeof(buffer));
			if (nbytes_recvd == -1) {
				if (conn_sock.GetLastError() == Socket::SOCKLIB_ETIMEDOUT) {
					// No data was available to receive.
				}
				else {
					perror("recv()");
					exit(1);
				}
			}
			// Expect data in a specific format --
			//     First, the number of game objects
			/*
			int num_gameobjects = 0;
			read_from_buffer(buffer, &num_gameobjects);
			std::cout << "Reading " << num_gameobjects << " objects.\n";
			game_objects.clear();
			game_objects.reserve(num_gameobjects);
			size_t buffer_offset = 0;

			for (int i = 0; i < num_gameobjects; i++) {
				GameObject* go = new GameObject;
				buffer_offset += DeserializeGameObjectFromBytes(go,
					&buffer[buffer_offset], sizeof(buffer) - buffer_offset);
				game_objects.push_back(go);
			}
			*/
		}
	}
}