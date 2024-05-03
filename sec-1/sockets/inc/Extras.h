#pragma once
#include "../PokemonCreation/Pokemon.h"

struct BattleEvent
{
	int client_id;
	int attackIndex;
};

struct Player
{
	int client_id;
	int leader;
	Pokemon party[3];
};

enum State
{
	CONNECTED,
	CHOOSE_ATTACKS,
	WAITING,
	DISPLAY_ATTACKS,
	WIN,
	LOST
};

// Source: https://stackoverflow.com/a/46931770
std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}
#pragma endregion