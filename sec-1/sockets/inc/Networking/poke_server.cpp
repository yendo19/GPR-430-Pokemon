#include "poke_server.h"

PokemonServer::PokemonServer(const char* host, int port)
{
	// TCP Server: Must pick STREAM for Type
	listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);
	listen_sock->Bind(Address(host, port));
	listen_sock->Listen();
	listen_sock->SetNonBlockingMode(true);
}

void PokemonServer::acceptConnections()
{
	std::cout << "Server: Waiting for connection...\n";
	conn_sock = new Socket(std::move(listen_sock->Accept()));
	std::cout << "Server: Got connection.\n";
	conn_sock->SetTimeout(.1);
	connection_alive = true;
}

PokemonServer::~PokemonServer()
{
	delete listen_sock;
	listen_sock = NULL;
}

void PokemonServer::update(float dt, int frame_num)
{
	while (connection_alive) {
		char buffer[4096];

		//std::cout << "Server: Connection live, processing packets\n";
		int nbytes_recvd = conn_sock->Recv(buffer, sizeof(buffer));
		if (nbytes_recvd == -1) {
			int error = conn_sock->GetLastError();
			if (error == Socket::SOCKLIB_ETIMEDOUT)
			{
				std::cout << "Server: Client timed out." << "\n";
				connection_alive = false; // Stop handling this connection and move on to a new connection.
				break;
			}
			else if (error == Socket::SOCKLIB_EWOULDBLOCK) {
				//std::cout << "Server: Received no message this frame.\n";
				break;
			}
			else {
				//perror("recv()\n");
				//exit(1);
				std::cerr << "Server: Unexpected error!\n";
				break;
			}
		}
		else if (nbytes_recvd == 0)
		{
			std::cout << "Server: Nothing received. Connection no longer alive.\n";
			connection_alive = false; // Stop handling this connection and move on to a new connection.
			return;
		}

		std::string recv_str(buffer, nbytes_recvd);
		std::cout << "Server: Received from client: " << recv_str << "\n";
	}

	// debug send some msg to client
	sendToClients("Hi clients!");

	// see what the msg was appended with to figure out what function
	// to cal on gamemanager
}

void PokemonServer::sendToClients(const char* data)
{
	std::cout << "Server: Sending packet to clients: " << data << "\n";
	conn_sock->Send(data, std::strlen(data));
}