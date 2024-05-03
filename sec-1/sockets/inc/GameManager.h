#pragma once
#include <list>
#include "../PokemonCreation/MyParty.h"
#include "../PokemonCreation/Attacks.h"

struct BattleEvent
{
	int attackIndex;
};

struct Player
{
	int client_id;
	myParty party;
};

class GameManager
{
private:
	std::list<Player> connected_players;

	// Round-related
	int current_round;
	std::list<BattleEvent> event_queue;

	static char* serializeBattleEvent(BattleEvent battleEvent);
	static BattleEvent deserializeBattleEvent(char* serialized_event);

public:

	GameManager();
	~GameManager();

	static void acceptAttackInput(BattleEvent battleEvent);

	// CALLED BY SERVER
	// called whenever the server receives a connection
	void trackPlayer(Player player);

	// CALLED BY CLIENT
	void sendEventToServer(BattleEvent battleEvent);

	// CALLED BY SERVER
	// this should be called whenever the server receives an event
	void queueEvent(char* serializedBattleEvent);

	// CALLED BY SERVER
	void broadcastEventsToClients();

	// CALLED BY CLIENTS
	// when they receive the packet for a pokemon to update
	void updateEntry(int ownerId, char* serializedPokemon);

	static GameManager & GetGameManager() {
		static GameManager gm;
		return gm;
	}
};

#pragma endregion