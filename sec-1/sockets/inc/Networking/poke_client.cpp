#include "poke_client.h"
#include "../../PokemonCreation/MyParty.h"
#include "../../socklib.h"
#include "../../PokemonCreation/Pokemon.h"
#include "../Extras.h"

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
	current_state = CONNECTED;
}

void PokemonClient::initParty()
{
	// initialize the party
	std::string filePath = "assets";
	//std::cout << filePath;
	std::list<Pokemon> temp;
	party = new myParty();
	party->Init(filePath);
	party->CreateFolder();
	party->updatePc();

	party->Update();
}

myParty PokemonClient::getParty()
{
	return *party;
}

PokemonClient::~PokemonClient()
{
	SockLibShutdown();
	// socket cleans up itself I think?
}

void PokemonClient::update(float dt, int frame_num) 
{
	sendToServer("Hi server!");

	std::string msg = receivePacket();

	if (msg.length() > 0)
	{
		std::cout << "Client: Received from server: " << msg << "\n";
	}
		

	// PROCESS WHAT MSG WE GOT BACK
	// parse the header of the packet
	processPacket(msg);
}

void PokemonClient::processPacket(std::string msg)
{
	bool is_valid = true;
	// split string by spaces
	std::vector<std::string> values = split(msg, ' ');

	// should have at least 2 values (LIST and one number)
	if (values.size() <= 1) { is_valid = false; }

	// first value should be LABEL
	switch (current_state)
	{
	case CONNECTED:
		if (values[0].compare("YOURID"))
		{
			client_id = std::stoi(values[1]);
			std::cout << "Client: Got my ID: " << client_id << "\n";

			// show connected UI....
		}
		if (values[0].compare("CHANGESTATE"))
		{
			if (values[1].compare("CHOOSE_ATTACKS"))
			{
				current_state = CHOOSE_ATTACKS;
				std::cout << "Client: Changing state to CHOOSE_ATTACKS.\n";

				// show our attack UI....

				// once we're clicked an attack button, we should move to 
				// current_state = WAITING;
				// and disable to attack choosing UI
			}
		}
		break;
	case CHOOSE_ATTACKS:
		if (values[0].compare("CHANGESTATE"))
		{
			if (values[1].compare("DISPLAY_ATTACKS"))
			{
				current_state = DISPLAY_ATTACKS;
				std::cout << "Client: Changing state to DISPLAY_ATTACKS.\n";

				// this calls once all players have submitted their attacks
				// at this point the client should've already updated
				// their entries as they have been receiving
				// battle events up to this point
			}
		}
		break;
	case DISPLAY_ATTACKS:
		if (values[0].compare("CHANGESTATE"))
		{
			if (values[1].compare("CHOOSE_ATTACKS"))
			{
				current_state = CHOOSE_ATTACKS;
				std::cout << "Client: Changing state to CHOOSE_ATTACKS.\n";

				// time for players to choose attacks again
				// after the server sends the signal to display attacks,
				// the server will wait like 10 seconds then
				// send the signal to choose attacks again
			}
		}
		break;
	}
	

	if (values[0].compare("BATTLEEVENT"))
	{
		
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




