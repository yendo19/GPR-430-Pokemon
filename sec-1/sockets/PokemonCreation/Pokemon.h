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
#include <iomanip>
#include <sstream>

class Pokemon
{
private:
	std::string _name;
	int _currentHealth;
	int _maxHealth;
	int _speed;
	Attack _myAttacks[4];
	std::string fileDir;

public:
	Pokemon()
	{
		_name = "";
		_maxHealth = 0;
		_currentHealth = 0;
		_speed = 0;
		fileDir = "";
	}

	Pokemon(std::string name, int hp, int speed , Attack atks[4], std::string file)
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
	
	Pokemon(std::list<char> serialized)
	{
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
	int getSpeed() { return _speed; }
	int getMaxHealth() { return _maxHealth; }
	Attack getAttackAt(int i) { return _myAttacks[i]; }
#pragma endregion

	
#pragma region File Creation / Reading
	void CreateFiles()
	{

		std::ofstream output(fileDir + "\\Pokemans\\" + _name + ".txt");

		if (output.is_open() && output.good())
		{
			output << _name << std::endl << _maxHealth << std::endl << _speed << std::endl;
			for each (Attack atk in _myAttacks)
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

	void readFiles(std::string fileName, std::string dir)
	{
		std::ifstream file(dir + "\\Pokemans\\" + fileName);
		std::cout << dir + "\\Pokemans\\" + fileName << std::endl;
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
		for each (Attack atk in _myAttacks)
		{
			std::cout << "    " << i << ": " << atk.getName() << std::setw(50) << std::right<< "     DMG: "  <<atk.getDamage() << std::endl;
			i++;
		}
	}
#pragma endregion

#pragma region Serialize/Deserialize
	char* serialize()
	{
		std::list<char> temp;
		std::stringstream num;
		std::string numbers;
		for (char c : _name)
			temp.push_back(c);

		temp.push_back(':');
		num.str("");
		num.clear();
		numbers.erase();
		num << _maxHealth;
		numbers = num.str();
		//std::cout << numbers << std::endl;
		for (char c : numbers)
			temp.push_back(c);

		temp.push_back(':');
		num.str("");
		num.clear();
		numbers.erase();
		num << _currentHealth;
		numbers = num.str();
		//std::cout << numbers << std::endl;
		for (char c : numbers)
			temp.push_back(c);

		temp.push_back(':');
		num.str("");
		num.clear();
		numbers.erase();
		num << _speed;
		numbers = num.str();
		//std::cout << numbers << std::endl;
		for (char c : numbers)
			temp.push_back(c);


		for (Attack atk : _myAttacks)
		{
			temp.push_back(':');
			for (char c : atk.getName())
				temp.push_back(c);
			temp.push_back(':');
			num.str("");
			num.clear();
			numbers.erase();
			num << atk.getDamage();
			numbers = num.str();
			//std::cout << numbers << std::endl;
			for (char c : numbers)
				temp.push_back(c);
		}
		
		// convert to array
		char** buffer = (char**)malloc(sizeof(char*) * temp.size());
		return *buffer;
	}

	std::list<std::string> split(const std::string& str) {
		std::list<std::string> temp;
		std::istringstream iss(str);
		std::string token;

		while (std::getline(iss, token, ':')) {
			temp.push_back(token);
		}

		return temp;
	}

	void deserialize(std::string ser)
	{
		int count = 0;
		std::list<std::string> holder = split(ser);
		
		for (std::string st : holder)
		{
			std::cout << st << std::endl;
		}


	}

	static void pickAttack(std::string data)
	{
		//TODO:send the chosen attack's power to the other player
		//for now, just print out the data
		std::cout << data;
	}

	static std::string serializeMove(Pokemon pkmn, Attack atk)
	{
		std::string temp;
		std::stringstream num;
		std::string numbers;

		num.str("");
		num << pkmn.getSpeed();
		numbers = num.str();
		for (char c : numbers)
			temp.push_back(c);
		temp.push_back(':');
		for (char c : atk.getName())
			temp.push_back(c);
		temp.push_back(':');
		num.str("");
		num.clear();
		numbers.erase();
		num << atk.getDamage();
		numbers = num.str();
		for (char c : numbers)
			temp.push_back(c);

		return temp;
	}
};

#pragma endregion


