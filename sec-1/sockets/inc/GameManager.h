#pragma once
#include <list>
#include "../PokemonCreation/MyParty.h"
#include "../PokemonCreation/Attacks.h"
#include "random"

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

class GameManager
{
private:
	std::list<Player> connected_players;

	// Round-related
	int current_round;
	std::list<BattleEvent> event_queue;

	static char* serializeBattleEvent(BattleEvent battleEvent);
	static BattleEvent deserializeBattleEvent(char* serialized_event);

	void evaluateRound();

	bool isServer;

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
	void updateEntry(int ownerId,int pokemonIndex,char* serializedPokemon);

	static GameManager & GetGameManager() {
		static GameManager gm;
		return gm;
	}

	Player* getPlayerAtIndex(size_t index);

	bool getIsServer() { return isServer; }

	void update()
	{
		if (!isServer) return;

		// do round:
		
		// ask clients to choose attacks


		// wait for the clients to send attack events to the server


		// once we have 2 events queued, it's time to move to processing events

		// broadcast the changes to the clients

		// check if anybody has won/lost


	}
};


#pragma endregion