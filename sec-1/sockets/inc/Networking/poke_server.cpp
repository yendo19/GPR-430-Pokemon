#include "poke_server.h"

PokemonServer::PokemonServer(const char* host, int port)
{
	std::cout << "Initializing server.\n";
	// TCP Server: Must pick STREAM for Type
	listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);
	listen_sock->Bind(Address(host, port));
	listen_sock->Listen();
	//listen_sock->SetNonBlockingMode(true);
	connections = 0;
}

void PokemonServer::acceptConnections()
{
	//std::thread player1(&PokemonServer::acceptConnection, this);
	//std::thread player2(&PokemonServer::acceptConnection, this);
	for (int i = 0; i < 2; ++i)
	{
		acceptConnection();
	}
}

void PokemonServer::acceptConnection()
{
	std::cout << "Server: Waiting for connection...\n";
	Socket* conn_sock = new Socket(std::move(listen_sock->Accept()));
	std::cout << "Server: Got connection.\n";
	conn_sock->SetTimeout(.1);
	connection_sockets.push_back(conn_sock);

	// track the player that just joined
	sendToClient(connections, "YOURID " + connections);
	connections++;
}


PokemonServer::~PokemonServer()
{
	delete listen_sock;
	listen_sock = NULL;
}

void PokemonServer::update(float dt, int frame_num)
{
	for each (Socket* conn_sock in connection_sockets)
	{
		char buffer[4096];

		//std::cout << "Server: Connection live, processing packets\n";
		int nbytes_recvd = conn_sock->Recv(buffer, sizeof(buffer));
		if (nbytes_recvd == -1) {
			int error = conn_sock->GetLastError();
			if (error == Socket::SOCKLIB_ETIMEDOUT)
			{
				std::cout << "Server: Client timed out." << "\n";
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
			return;
		}

		std::string recv_str(buffer, nbytes_recvd);
		std::cout << "Server: Received from client: " << recv_str << "\n";
	}

	// debug send some msg to client
	sendToAllClients("Hi clients!");

	// see what the msg was appended with to figure out what function
	// to cal on gamemanager
}

void PokemonServer::sendToAllClients(const char* data)
{
	std::cout << "Server: Sending packet to clients: " << data << "\n";
	for (int i = 0; i < connection_sockets.size(); ++i)
	{
		connection_sockets[i]->Send(data, std::strlen(data));
	}
}

void PokemonServer::sendToClient(int client_id, const char* data)
{
	std::cout << "Server: Sending packet to client " << client_id << ": " << data << "\n";
	connection_sockets[client_id]->Send(data, std::strlen(data));
}