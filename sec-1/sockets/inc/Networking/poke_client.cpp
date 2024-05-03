#include "poke_client.h"
#include "../../PokemonCreation/MyParty.h"

//std::string getCurrentLocation() {
//	std::filesystem::path main_cpp_path = __FILE__;
//	std::filesystem::path main_cpp_folder = main_cpp_path.parent_path();
//	return main_cpp_folder.string();
//}

PokemonClient::PokemonClient(const char* host, int port)
{
	initParty();

	*message_buffer = { 0 };
	
	SockLibInit();
	// init our socket
	Address addr(host, port);
	connected_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);
	connected_sock->Connect(addr);
	connected_sock->SetNonBlockingMode(true);

	std::cout << "Connected to server!\n";
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

	// Did anyone send anything back to us?
	int nbytes_recvd = connected_sock->Recv(message_buffer, sizeof(message_buffer));
	if (nbytes_recvd == -1) {
		if (connected_sock->GetLastError() == Socket::SOCKLIB_EWOULDBLOCK) {
			to_display = "Client revieved no message this frame.\n";
		}
		else {
			std::cerr << "Client: Unexpected error!\n";
			return;
		}
	}
	else if (nbytes_recvd == 0) {
		std::cerr << "Client: Connection unexpectedly closed!\n";
		return;
	}
	else {

		// PROCESS WHAT MSG WE GOT BACK
		to_display = std::string(message_buffer, nbytes_recvd);
	}

	std::cout << "Client: Received from server: " << to_display << "\n";
}

void PokemonClient::sendToServer(const char* data)
{
	std::cout << "Client: Sending packet to server: " << data << "\n";
	connected_sock->Send(data, std::strlen(data));
}


