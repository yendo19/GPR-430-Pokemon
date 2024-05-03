#include "poke_client.h"
#include "../../PokemonCreation/MyParty.h"
#include "../../socklib.h"
#include "../../PokemonCreation/Pokemon.h"
#include "../Extras.h"
#include "../GameManager.h"

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

myParty* PokemonClient::getParty()
{
	return party;
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
		std::vector<std::string> packets = split(msg, '@');
		for (int i = 0; i < packets.size(); i++)
		{
			std::string packet = packets[i];
			if (packet.length() == 0) continue;
			std::cout << "Client: Received from server: " << packet << "\n";
			// PROCESS WHAT MSG WE GOT BACK
			processPacket(packet);
		}
	}
		
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
		if (values[0] == ("YOURID"))
		{
			client_id = std::stoi(values[1]);
			std::cout << "Client: Got my ID: " << client_id << "\n";

			// show connected UI....
			// .......

			std::string outmsg = std::to_string(client_id) + " PARTYSETUP " + party->getPokemonInPartyAt(0).serialize() + " " + party->getPokemonInPartyAt(1).serialize() + " " + party->getPokemonInPartyAt(2).serialize();
			sendToServer(outmsg.data());
		}
		if (values[0] ==("CHANGESTATE"))
		{
			if (values[1] == ("CHOOSE_ATTACKS"))
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
		if (values[0] == ("CHANGESTATE"))
		{
			if (values[1] == ("DISPLAY_ATTACKS"))
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
		if (values[0] == ("CHANGESTATE"))
		{
			if (values[1] == ("CHOOSE_ATTACKS"))
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
	

	if (values[0] == ("BATTLEEVENT"))
	{
		std::string id = values[1], attack = values[2];
		if (id == "9")
		{
			//missed
		}
		else
		{
			GameManager::GetGameManager().updateEntry(std::stoi(id), GameManager::GetGameManager().getOtherPlayer(std::stoi(id))->leader, GameManager::GetGameManager().getPlayerAtIndex(std::stoi(id))->party[GameManager::GetGameManager().getPlayerAtIndex(std::stoi(id))->leader].getAttackAt(std::stoi(attack)).getDamage());
		}
	}

	if (values[0] == "PARTYSETUP")
	{

		Player p_server;
		p_server.client_id = 0;
		p_server.leader = 0;

		p_server.party[0] = Pokemon::deserialize(values[1]);
		p_server.party[1] = Pokemon::deserialize(values[2]);
		p_server.party[2] = Pokemon::deserialize(values[3]);

		GameManager::GetGameManager().trackPlayer(p_server);

		Player p_local;
		p_local.client_id = 1;
		p_local.leader = 0;


		p_local.party[0] = party->getPokemonInPartyAt(0);
		p_local.party[1] = party->getPokemonInPartyAt(1);
		p_local.party[2] = party->getPokemonInPartyAt(2);

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

void PokemonClient::sendToServer(std::string data)
{
	std::cout << "Client: Sending packet to server: " << data << "\n";
	data = "@" + data;
	connected_sock->Send(data.c_str(), data.length());
}




