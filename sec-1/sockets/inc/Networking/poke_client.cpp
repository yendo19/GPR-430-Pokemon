#include "poke_client.h"
#include "../../PokemonCreation/MyParty.h"

//std::string getCurrentLocation() {
//	std::filesystem::path main_cpp_path = __FILE__;
//	std::filesystem::path main_cpp_folder = main_cpp_path.parent_path();
//	return main_cpp_folder.string();
//}

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

	if(msg.length() > 0)
		std::cout << "Client: Received from server: " << msg << "\n";

	// parse the header of the packet
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

		// PROCESS WHAT MSG WE GOT BACK
		to_display = std::string(message_buffer, nbytes_recvd);
	}
	return to_display;
}

void PokemonClient::sendToServer(const char* data)
{
	std::cout << "Client: Sending packet to server: " << data << "\n";
	connected_sock->Send(data, std::strlen(data));
}


