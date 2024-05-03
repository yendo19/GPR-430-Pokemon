#include "GameManager.h"

char* GameManager::serializeBattleEvent(BattleEvent battleEvent)
{
	// to be implemented
	std::string temp = std::to_string(battleEvent.client_id) + ':' + std::to_string(battleEvent.attackIndex);
	const int length = temp.length();

	char* arr = new char[length + 1];
	std::strcpy(arr, temp.c_str());

	return arr;
}

BattleEvent GameManager::deserializeBattleEvent(char* serialized_event)
{
	std::string id;
	std::string atkInd;
	int size = sizeof(serialized_event);
	for (int i = 0; i < size/4; i++)
	{
		while (serialized_event[i] != ':')
		{
			id += serialized_event[i];
		}
		atkInd = serialized_event[i];
	}

	BattleEvent temp;
	temp.attackIndex = std::stoi(atkInd);
	temp.client_id = std::stoi(id);

	return temp;
}


//to be called when both players have submitted a BattleEvent to the event_queue
void GameManager::evaluateRound()
{	
	//Only run on the host
	if (!getIsServer())
		return;
	for (BattleEvent move : event_queue)
	{
		for (Player p : connected_players)
		{
			if (p.client_id != move.client_id)
			{
				Player* attacked = getPlayerAtIndex(move.client_id);

				//If this is false, you have missed, skill issue
				if (120 - p.party[p.leader].getAttackAt(move.attackIndex).getDamage() > std::rand() % 100)
				{
					attacked->party[attacked->leader].applyDamage(p.party[p.leader].getAttackAt(move.attackIndex).getDamage());
				}
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
void GameManager::updateEntry(int ownerId, int pokemonIndex, char* serializedPokemon)
{
	// deserialized the pokemon
	Pokemon temp;
	std::string str(serializedPokemon, sizeof(serializedPokemon));
	temp.deserialize(str);
	// updates the local copy of the chosen pokemon
	getPlayerAtIndex(ownerId)->party[pokemonIndex] = temp;
}

Player* GameManager::getPlayerAtIndex(size_t index)
{
	std::list<Player>::iterator nth = connected_players.begin();
	std::advance(nth, index);

	return &*nth;
}