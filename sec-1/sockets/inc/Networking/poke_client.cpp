#include "poke_client.h"
#include "../../PokemonCreation/MyParty.h"

// Source: https://stackoverflow.com/a/46931770
std::vector<std::string> split(const std::string& s, char delim) {
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

	// first value should be LIST
	if (values[0].compare("YOURID")) 
	{
		client_id = std::stoi(values[1]);
		std::cout << "Client: Got my ID: " << client_id << "\n";
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




