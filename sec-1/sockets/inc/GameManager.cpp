#include "GameManager.h"

char* GameManager::serializeBattleEvent(BattleEvent battleEvent)
{
	// to be implemented
	return nullptr;
}

BattleEvent GameManager::deserializeBattleEvent(char* serialized_event)
{
	// to be implemented

	return BattleEvent();
}

void GameManager::evaluateRound()
{	
	if (!getIsServer())
		return;
	for (BattleEvent move : event_queue)
	{
		for (Player p : connected_players)
		{
			if (p.client_id != move.client_id)
			{
				//connected_players[move.client_id] move.attackIndex
				//if ()
				//p.party[p.leader].
			}
		}
	}
}

GameManager::GameManager()
{
	connected_players = std::list<Player>();
	current_round = 0;
	event_queue = std::list<BattleEvent>();
}

GameManager::~GameManager()
{
}

void GameManager::acceptAttackInput(BattleEvent battleEvent)
{
	GetGameManager().broadcastEventsToClients();
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
	for (int i = 0; i < event_queue.size(); ++i)
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

Player GameManager::getPlayerAtIndex(size_t index)
{
	std::list<Player>::iterator nth = connected_players.begin();
	std::advance(nth, index);

	return *nth;
}