#include <string>
#include "../socklib.h"
#include "../defer.h"

class PokemonClient
{
private:
	Socket* connected_sock;
	Address address;

	PokemonClient(const char* host, int port);
	
public:
	std::string do_client(std::istream& in_stream);
};


