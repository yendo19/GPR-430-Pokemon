#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>

class attacks
{
public:
	std::string  attackName;
	int damage;

	void setName(std::string name) { attackName = name; }
	void setDamage(int damages) { damage = damages; }

	std::string getName() { return attackName; }
	int getDamage() { return damage; }
};


