#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "Pokemon.h"

class myParty
{
public:
	void Init();
	void Update();
	void CreateFiles();
	std::list<std::string> CheckForFiles();
	void updatePC();
	void CreateFolder();

private:
	Pokemon _myPokemon[3];
	std::list<Pokemon> PC;
};


