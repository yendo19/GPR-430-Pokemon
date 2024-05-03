#include "server.h"

PokemonServer::PokemonServer(const char* host, int port)
{
	// TCP Server: Must pick STREAM for Type
	listen_sock = new Socket(Socket::Family::INET, Socket::Type::STREAM);

	// Next: Bind to an address (same as UDP server)
	Address srv_addr(host, port);
	listen_sock->Bind(srv_addr);
	listen_sock->SetNonBlockingMode(true);
}

PokemonServer::~PokemonServer()
{
	delete listen_sock;
	listen_sock = NULL;
}

void PokemonServer::run_server()
{
	// Next: Listen() on socket
	// This makes this socket into a "listen socket",
	// which means you're no longer allowed to call
	// send/recv on this socket -- only accept().
	// Begin listening on the server socket (use a backlog parameter of 20).
	listen_sock->Listen(20);

	// Accept connections from clients forever -- if someone wants
	// to shut the server down, they can use the task manager.
	while (true)
	{
		// Next step is to accept() connections.
		// Returns a socket representing the connection,
		// which can have send(), recv() and close()
		// called on it without disrupting the listen
		// socket.
		std::cout << "Waiting for connection...\n";
		Socket conn_sock = listen_sock->Accept();
		std::cout << "Received connection!\n";

		// Now, keep talking to the client until
		// they shut down the connection.
		bool is_connected = true;
		while (is_connected)
		{
			// receive data from client
			static char buffer[4096];
			int nbytes_recvd = conn_sock.Recv(buffer, sizeof(buffer));
			std::string response;

			// RECEIVE DATA =============================================

			// Check for errors!
			// nbytes_recvd == -1 indicates an error.
			if (nbytes_recvd == -1)
			{
				perror("recv()"); // Print the system error message.
				exit(1); // Don't wanna deal with errors -- just crash.
			}

			// Next, check to see if the connection was closed.
			// nbytes_recvd == 0 indicates other end hung up.
			if (nbytes_recvd == 0)
			{
				is_connected = false; // Stop handling this connection and move on to a new connection.
				break;
			}

			/*
			Parse the data provided by the client to ensure that it is valid:
			- Assume that the data is encoded using ASCII -- it will only send values in the range [0, 127].
			- The string starts with "LIST" followed by a space
			- "LIST" is followed by a list of items (only integers and floating-point numbers are allowed!)
			- If the data is found to be invalid for any reason, return the string "ERROR" back to the user.
			*/

			std::string msg_str(buffer, nbytes_recvd);
			std::cout << "Received: " << msg_str << "\n";
			std::string option = "";
			bool is_valid = true;

			// CHECK OPTIONS =============================================

			// make sure appended by round number

			// validate data

			// CONSTRUCT RESPONSE STRING ===================================

			// placeholder
			response = "hi";

			// SEND RESPONSE DATA ==========================================
			std::cout << "Sending: " << response << "\n";
			conn_sock.Send(response.data(), response.size());

			is_connected = true;
		}
		std::cout << "Connection hung up.\n";
	}
}