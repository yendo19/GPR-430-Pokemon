#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include "Pokemon.h"
#include <list>
#include <filesystem>
#include <iostream>
#include <fstream>

class Attack
{
public:
	Attack()
	{
		_atkName = "";
		_damage = 0;
	}

	Attack(std::string name, int dmg)
	{
		_atkName = name;
		_damage = dmg;
	}
	~Attack()
	{

	}

#pragma region File read/write
	std::list<std::string> writeToFile()
	{
		std::list<std::string> temp;
		temp.push_back(_atkName);
		temp.push_back(std::to_string(_damage));
		return temp;
	}
#pragma endregion
#pragma region getter/setters
	void setName(std::string name) { _atkName = name; }
	void setDamage(int damages) { _damage = damages; }

	std::string getName() { return _atkName; }
	int getDamage() { return _damage; }
#pragma endregion


	private:
		std::string fileDir;
		std::string _atkName;
		int _damage;
};


