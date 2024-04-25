#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
//#include "Pokemon.h"
#include <list>


class myParty
{
public:
	myParty() {};
	~myParty() {};
	void Init();
	void Update();
	void CreateFiles();
	std::list<std::string> CheckForFiles();
	//void updatePC();
	void CreateFolder();

private:
	//Pokemon _myPokemon[3];
	//std::list<Pokemon> PC;
	std::string fileDir;
};


