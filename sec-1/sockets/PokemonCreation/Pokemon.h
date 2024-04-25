#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include "Pokemon.h"
#include "Attacks.h"
#include <list>
#include <filesystem>
#include <iostream>
#include <fstream>

class Pokemon
{
public:
	Pokemon(std::string name, int hp /*, atks[4]*/, std::string file)
	{
		_name = name;
		_maxHealth = hp;
		_currentHealth = hp;
		//_myAttacks[4] = NULL;
		fileDir = file;
	}

	Pokemon(std::string file)
	{
		_name = "";
		_maxHealth = 0;
		_currentHealth = 0;
		//_myAttacks[4] = NULL;
		fileDir = file;
	}
	~Pokemon() {};
	
	std::string getName() { return _name; }
	int getHealth() { return _currentHealth; }
	int getMaxHealth() { return _maxHealth; }
	attacks getAttackAt(int i) { return _myAttacks[i]; }
	

	void CreateFiles()
	{
		std::ofstream output(fileDir + "\\Pokemans\\" + _name + ".txt");
		if (output.is_open())
		{
			output << _name << std::endl << _maxHealth << std::endl;
			output.close();
			std::cout << "Created: "  << _name << std::endl;
		}
		else
		{
			//std::cout << "killme";
		}
	}


	void readFiles(std::string fileName)
	{
		std::ifstream file(fileDir + "\\Pokemans\\" + fileName);
		if (file.is_open())
		{
			file >> _name;
			file >> _maxHealth;
			_currentHealth = _maxHealth;
			/*Attacks too*/
		}
		else
		{
			std::cout << "help";
		}

	}

private:
	std::string _name;
	int _currentHealth;
	int _maxHealth;
	attacks _myAttacks[4];
	std::string fileDir;
};



