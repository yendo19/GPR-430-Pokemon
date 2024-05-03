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

	//address = Address(host, port);
	// init our socket
	//start_client();
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

void PokemonClient::start_client()
{
	connected_sock->Create(Socket::Family::INET, Socket::Type::STREAM);
	connected_sock->Connect(address);
	connected_sock->SetNonBlockingMode(true);

	std::cout << "Connected to server!\n";
}

PokemonClient::~PokemonClient()
{
	//delete connected_sock;
	//connected_sock = NULL;
}


// PLACEHOLDER "UPDATE" - NEEDS TO BE REMOVED
std::string PokemonClient::do_client()
{

	// send data (this is the same interface as file.write();
	// Create a string to send to the server using the build_string() function
	std::string msg_to_send = "some string";
	size_t nbytes_sent = connected_sock->Send(msg_to_send.data(), msg_to_send.size());

	std::cout << "Sent " << nbytes_sent << "bytes to server" << "\n";

	// let's see if Scott's server has anything to say back to us..
	// Recv = Receive
	// Recv is a blocking operation (it waits for data to come back to it)
	char buffer[4096];
	size_t nbytes_recved = connected_sock->Recv(buffer, sizeof(buffer));

	// Now, buffer has nbytes_recved written into it
	std::string msg_recved(buffer, nbytes_recved);
	std::cout << "Server says '" << msg_recved << "'\n";

	// return server's response
	return msg_recved;
}

void PokemonClient::Update(float dt, int frame_num) {

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