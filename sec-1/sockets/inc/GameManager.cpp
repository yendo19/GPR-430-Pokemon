#include "GameManager.h"

static char* GameManager::serializeBattleEvent(BattleEvent battleEvent)
{
	// to be implemented
}

static void GameManager::deserializeBattleEvent(char* serialized_event)
{
	// to be implemented

	// sample
	BattleEvent battleEvent =
	{
		0, //int target_client_id;
		1, //int target_pokemon;
		-10, //int health_change;
	};

}

// CALLED BY SERVER
	// called whenever the server receives a connection
void GameManager::trackPlayer(Player player)
{
	connected_players.push_back(player);
}

// CALLED BY CLIENT
void GameManager::sendEventToServer(BattleEvent battleEvent)
{
	// serializes event
	char* data = serializeBattleEvent(battleEvent);

	// sends the packet to the server
	// ........
}

// CALLED BY SERVER
// this should be called whenever the server receives an event
void GameManager::queueEvent(char* serializedBattleEvent)
{
	BattleEvent battleEvent = deserializeBattleEvent(serializedBattleEvent);
	event_queue.push_back(battleEvent);
}

// CALLED BY SERVER
void GameManager::broadcastEventsToClients()
{
	// process all events that are queued (this happens at the end of each round)
	for (int i = 0; i < event_queue.size; ++i)
	{
		// determine the new health of target pokemon

		// send packet to client notifying of the new pokemon states
	}
}

// CALLED BY CLIENTS
// when they receive the packet for a pokemon to update
void GameManager::updateEntry(int ownerId, char* serializedPokemon)
{
	// deserialized the pokemon

	// updates the local copy of the chosen pokemon
}