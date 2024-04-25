#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include "Attacks.h"
#include <list>
#include <filesystem>
#include <iostream>
#include <fstream>


class Pokemon
{
public:
	Pokemon(std::string name, int hp, int speed , attacks atks[4], std::string file)
	{
		_name = name;
		_maxHealth = hp;
		_currentHealth = hp;
		_speed = speed;
		//_myAttacks[4] = NULL;
		fileDir = file;

		for (int i = 0; i < 4; i++)
		{
			_myAttacks[i] = atks[i];
		}
	}

	Pokemon(std::string file)
	{
		_name = "";
		_maxHealth = 0;
		_currentHealth = 0;
		// _myAttacks[4] = NULL;
		fileDir = file;
		_speed = 0;
	}
	~Pokemon() {};
#pragma region Data Accessors
	std::string getName() { return _name; }
	int getHealth() { return _currentHealth; }
	int getMaxHealth() { return _maxHealth; }
	attacks getAttackAt(int i) { return _myAttacks[i]; }
#pragma endregion

	
#pragma region File Creation / Reading
	void CreateFiles()
	{

		std::ofstream output(fileDir + "\\Pokemans\\" + _name + ".txt");

		if (output.is_open() && output.good())
		{
			output << _name << std::endl << _maxHealth << std::endl << _speed << std::endl;
			for each (attacks atk in _myAttacks)
			{
				output << atk.getName() << std::endl << atk.getDamage() << std:: endl;
			}
			output.close();
			std::cout << "Created: " << _name << std::endl;
		}
		else
		{
			std::cout << "File creation failed or Pokemon with that name already exists";
		}
	}

	void readFiles(std::string fileName)
	{
		std::ifstream file(fileDir + "\\Pokemans\\" + fileName);
		if (file.is_open())
		{
			std::string temp;
			file >> _name;
			file >> _maxHealth;
			file >> _speed;
			_currentHealth = _maxHealth;
			for (int i = 0; i < 4; i++)
			{
				file >> temp;
				_myAttacks[i].setName(temp);
				file >> temp;
				_myAttacks[i].setDamage(std::stoi(temp.c_str()));
			}
		}
		else
		{
			std::cout << "help";
		}

	}
#pragma endregion

#pragma region cout
	void coutData()
	{
		int i = 0;
		std::cout << _name << "    HP: " << _maxHealth << "    Speed: " << _speed << std::endl;
		std::cout << "Attacks:\n";
		for each (attacks atk in _myAttacks)
		{
			std::cout << "    " << i << ": " << atk.getName() << "     DMG: " << atk.getDamage() << std::endl;
			i++;
		}
	}
#pragma endregion


private:
	std::string _name;
	int _currentHealth;
	int _maxHealth;
	int _speed;
	attacks _myAttacks[4];
	std::string fileDir;
};



