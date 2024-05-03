#include "GameManager.h"
#include "../PokemonCreation/MyParty.h"
#include "../PokemonCreation/Attacks.h"
#include "./Networking/poke_server.h"
#include "./Networking/poke_client.h"
#include "Extras.h"

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
		// ask the server to send the msg to all clients
		server->sendToAllClients(serializeBattleEvent(move));

		/*
		for (Player p : connected_players)
		{
			if (p.client_id != move.client_id)
			{
				Player* attacked = getPlayerAtIndex(move.client_id);

				//If this is false, you have missed, skill issue
				if (120 - p.party[p.leader].getAttackAt(move.attackIndex).getDamage() > std::rand() % 100)
				{
					attacked->party[attacked->leader].applyDamage(p.party[p.leader].getAttackAt(move.attackIndex).getDamage());
					updateEntry(attacked->client_id, attacked->leader, attacked->party[attacked->leader].serialize());
				}
			}
		}
		*/
	}
}

GameManager::GameManager()
{
	connected_players = std::list<Player>();
	current_round = 0;
	event_queue = std::list<BattleEvent>();
	deltaTime = 0;
}

GameManager::~GameManager()
{
}

void GameManager::update(float dt, int frame_num)
{
	local_client->update(dt, frame_num);

	if (!isServer) return;
	server->update(dt, frame_num);
	// do round:

	// once the server has accepted all connections,
	// it ask clients to choose attacks
	switch (server->getState())
	{
	case CHOOSE_ATTACKS:
		// wait for all clients to send attack events to the server...

		// once we have 2 events queued, it's time to move to processing events
		if (event_queue.size() == 2)
		{
			// broadcast the changes to the clients
			broadcastEventsToClients();
			event_queue.clear(); // clear the queue
			phaseStartTime = (float)clock() / CLOCKS_PER_SEC;

			setState(State::DISPLAY_ATTACKS);
			std::string msg = "CHANGESTATE DISPLAY_ATTACKS";
			server->sendToAllClients(msg.c_str());
		}
		break;
	case DISPLAY_ATTACKS:
		// wait like 10 seconds before changing back to choose attacks
		deltaTime = ((float)clock() / CLOCKS_PER_SEC) - phaseStartTime;

		if (deltaTime > 10.0f)
		{
			// tell the servers to choose attacks again
			std::string msg = "CHANGESTATE CHOOSE_ATTACKS";
			server->sendToAllClients(msg.c_str());
		}
		break;
	}

	// check if anybody has won/lost
	if (int i = checkLoss() != -1)
	{
		//index at i lost
	}

}

int GameManager::checkLoss()
{
	for each (Player play in connected_players)
	{
		for each (Pokemon pkm in play.party)
		{
			if (pkm.getHealth() <= 0)
				return play.client_id;
		}
	}
	return -1;
}

void GameManager::setState(State state)
{
	server->setState(state);
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
	local_client->sendToServer(data);
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
	for (BattleEvent move : event_queue)
	{
		// ask the server to send the msg to all clients
		server->sendToAllClients(serializeBattleEvent(move));
	}
	evaluateRound();
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

Player* GameManager::getOtherPlayer(size_t index)
{
	if (index == 0)
		index = 1;
	else
		index = 0;

	std::list<Player>::iterator nth = connected_players.begin();
	std::advance(nth, index);

	return &*nth;
}