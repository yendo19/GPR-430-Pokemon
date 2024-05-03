#include "client.h"

PokemonClient::PokemonClient(const char* host, int port)
{
	// create a socket that we will use to listen to the server
	connected_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

	// created a socket, but it doesn’t talk to anything yet.
	std::string str_address("127.0.0.1"); // our pc?
	address = Address(str_address, 36925);

	// connect the socket to the given address
	connected_sock->Connect(address); // should match the server's open port

	std::cout << "Connected to " << str_address << "!\n";
}

std::string PokemonClient::do_client(std::istream& in_stream)
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