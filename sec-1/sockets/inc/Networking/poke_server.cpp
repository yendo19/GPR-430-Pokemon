#include "poke_server.h"
#include "../GameManager.h"
#include "../Extras.h"

PokemonServer::PokemonServer(const char* host, int port)
{
	std::cout << "Initializing server.\n";
	// TCP Server: Must pick STREAM for Type
	listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);
	listen_sock->Bind(Address(host, port));
	listen_sock->Listen();
	//listen_sock->SetNonBlockingMode(true);
	connections = 0;
	state = CONNECTED;
}

void PokemonServer::acceptConnections()
{
	for (int i = 0; i < 2; ++i)
	{
		acceptConnection();
	}

	// ALL CONNECTIONS HAVE BEEN RECEIVED!!
	setState(CHOOSE_ATTACKS);

	//send signal to clients to choose attacks
	std::string msg = "CHANGESTATE CHOOSE_ATTACKS";
	sendToAllClients(msg.c_str());
}

void PokemonServer::acceptConnection()
{
	std::cout << "Server: Waiting for connection...\n";
	Socket* conn_sock = new Socket(std::move(listen_sock->Accept()));
	std::cout << "Server: Got connection.\n";
	conn_sock->SetTimeout(.1);
	connection_sockets.push_back(conn_sock);

	// track the player that just joined
	sendToClient(connections, std::string("YOURID " + std::to_string(connections)).data());
	connections++;


}


PokemonServer::~PokemonServer()
{
	delete listen_sock;
	listen_sock = NULL;
}

void PokemonServer::update(float dt, int frame_num)
{
	for each (Socket * conn_sock in connection_sockets)
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
		
		if (recv_str.length() > 0)
		{
			std::vector<std::string> packets = split(recv_str, '@');

			for (int i = 0; i < packets.size(); i++)
			{
				std::string packet = packets[i];
				if (packet.length() == 0) continue;
				std::cout << "Server: Received from client: " << packet << "\n";
				// PROCESS WHAT MSG WE GOT
				processPacket(packet);
			}
		}
	}
}

void PokemonServer::processPacket(std::string msg)
{
	bool is_valid = true;
	// split string by spaces
	std::vector<std::string> values = split(msg, ' ');

	// should have at least 2 values (LIST and one number)
	if (values.size() <= 1) { is_valid = false; }

	// first value should be SENDER ID
	int senderId = std::stoi(values[0]);
	
	// second value should be the HEADER
	if (values[1] == ("BATTLEEVENT"))
	{
		// do somethign....
	}
	else if (values[1] == ("PARTYSETUP"))
	{

		Player p;
		p.client_id = senderId;
		p.leader = 0;

		p.party[0].deserialize(values[2], "");
		p.party[1].deserialize(values[3], "");
		p.party[2].deserialize(values[4], "");

		GameManager::GetGameManager().trackPlayer(p);
	}
}

void PokemonServer::sendToAllClients(std::string data)
{
	std::cout << "Server: Sending packet to clients: " << data << "\n";
	data = "@" + data;
	for (int i = 0; i < connection_sockets.size(); ++i)
	{
		connection_sockets[i]->Send(data.c_str(), data.length());
	}
}

void PokemonServer::sendToClient(int client_id, std::string data)
{
	std::cout << "Server: Sending packet to client " << client_id << ": " << data << "\n";
	data = "@" + data;
	connection_sockets[client_id]->Send(data.c_str(), data.length());
}