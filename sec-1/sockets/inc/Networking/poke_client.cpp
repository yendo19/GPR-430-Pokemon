#include "poke_client.h"
#include "../../PokemonCreation/MyParty.h"
#include "../GameManager.h"

// Source: https://stackoverflow.com/a/46931770
std::vector<std::string> PokemonClient::split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

PokemonClient::PokemonClient(const char* host, int port)
{
	std::cout << "Initializing client.\n";
	initParty();

	*message_buffer = { 0 };
	
	SockLibInit();
	// init our socket
	Address addr(host, port);
	connected_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);
	connected_sock->Connect(addr);
	connected_sock->SetNonBlockingMode(true);

	std::cout << "Client: Connected to server!\n";
}

void PokemonClient::initParty()
{
	// initialize the party
	std::string filePath = "assets";
	//std::cout << filePath;
	std::list<Pokemon> temp;
	party = myParty();
	party.Init(filePath);
	party.CreateFolder();
	party.updatePc();

	party.Update();
}

PokemonClient::~PokemonClient()
{
	SockLibShutdown();
	// socket cleans up itself I think?
}

void PokemonClient::update(float dt, int frame_num) 
{
	std::string msg = receivePacket();

	if (msg.length() > 0)
	{
		std::cout << "Client: Received from server: " << msg << "\n";

		// PROCESS WHAT MSG WE GOT BACK
		// parse the header of the packet
		processPacket(msg);
	}
		
}

void PokemonClient::processPacket(std::string msg)
{
	bool is_valid = true;
	// split string by spaces
	std::vector<std::string> values = split(msg, ' ');

	// should have at least 2 values (LIST and one number)
	if (values.size() <= 1) { is_valid = false; }

	// first value should be LIST
	if (values[0].compare("YOURID")) 
	{
		client_id = std::stoi(values[1]);
		std::cout << "Client: Got my ID: " << client_id << "\n";
		std::string outmsg = std::to_string(client_id) + " PARTYSETUP " + party.getPokemonInPartyAt(0).serialize() + " " + party.getPokemonInPartyAt(1).serialize() + " " + party.getPokemonInPartyAt(2).serialize();
		sendToServer(outmsg.data());
	}

	if (values[0].compare("BATTLEEVENT"))
	{
		
	}

	else if (values[0].compare("PARTYSETUP"))
	{

		Player p_server;
		p_server.client_id = 0;
		p_server.leader = 0;

		p_server.party[0].deserialize(values[1], "");
		p_server.party[1].deserialize(values[2], "");
		p_server.party[2].deserialize(values[3], "");

		GameManager::GetGameManager().trackPlayer(p_server);

		Player p_local;
		p_local.client_id = 1;
		p_local.leader = 0;

		p_local.party[0] = party.getPokemonInPartyAt(0);
		p_local.party[1] = party.getPokemonInPartyAt(1);
		p_local.party[2] = party.getPokemonInPartyAt(2);

		GameManager::GetGameManager().trackPlayer(p_local);
	}
}

std::string PokemonClient::receivePacket()
{
	std::string to_display = "";

	// Did anyone send anything back to us?
	int nbytes_recvd = connected_sock->Recv(message_buffer, sizeof(message_buffer));
	if (nbytes_recvd == -1) {
		if (connected_sock->GetLastError() == Socket::SOCKLIB_EWOULDBLOCK) {
			//to_display = "Client revieved no message this frame.\n";
		}
		else {
			std::cerr << "Client: Unexpected error!\n";
			return to_display;
		}
	}
	else if (nbytes_recvd == 0) {
		std::cerr << "Client: Connection unexpectedly closed!\n";
		return to_display;
	}
	else {
		to_display = std::string(message_buffer, nbytes_recvd);
	}
	return to_display;
}

void PokemonClient::sendToServer(const char* data)
{
	std::cout << "Client: Sending packet to server: " << data << "\n";
	connected_sock->Send(data, std::strlen(data));
}




