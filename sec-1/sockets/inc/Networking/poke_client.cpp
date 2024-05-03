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

void PokemonClient::update(float dt, int frame_num) {

	// Don't need to update this module _every_ frame...
	// Every other frame is plenty.
	if (frame_num % 2 == 0) return;

	for (Pokemon go : party.getParty()) {
		connected_sock->Send(go.serialize(), sizeof(Pokemon));
	}

	// Do we have to send out? Then send it.
	if (rand() % 4 != 0) {
		std::stringstream ss;
		ss << "LIST";
		for (int i = 0; i < 5; i++) {
			ss << " " << rand() % 500;
		}

		std::string to_send = ss.str();
		connected_sock->Send(to_send.c_str(), to_send.size());
	}

	// Did anyone send anything back to us?
	int nbytes_recvd = connected_sock->Recv(message_buffer, sizeof(message_buffer));
	if (nbytes_recvd == -1) {
		if (connected_sock->GetLastError() == Socket::SOCKLIB_EWOULDBLOCK) {
			to_display = "No message this frame.\n";
		}
		else {
			std::cerr << "Unexpected error!\n";
			abort();
		}
	}
	else if (nbytes_recvd == 0) {
		std::cerr << "Connection unexpectedly closed!\n";
		abort();
	}
	else {
		to_display = std::string(message_buffer, nbytes_recvd);
	}
}