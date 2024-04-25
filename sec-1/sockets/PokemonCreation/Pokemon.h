#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "Attacks.h"

class Pokemon
{
public:
	std::string getName() { return _name; }
	int getHealth() { return _health; }
	attacks getAttackAt(int i) { return _myAttacks[i]; }

private:
	std::string _name;
	int _health;
	attacks _myAttacks[4];
};



