#include "GameManager.h"
#include "../PokemonCreation/MyParty.h"
#include "../PokemonCreation/Attacks.h"
#include "./Networking/poke_server.h"
#include "./Networking/poke_client.h"
#include "Extras.h"

char* GameManager::serializeBattleEvent(BattleEvent battleEvent)
{
	// to be implemented
	std::string temp = "BATTLEEVENT:" + std::to_string(battleEvent.client_id) + ':' + std::to_string(battleEvent.attackIndex);
	const int length = temp.length();

	char* arr = new char[length + 1];
	std::strcpy(arr, temp.c_str());

	return arr;
}

BattleEvent GameManager::deserializeBattleEvent(const char* serialized_event)
{
	std::string id;
	std::string atkInd;
	int size = sizeof(serialized_event);
	std::string str(serialized_event, sizeof(serialized_event));
	std::vector<std::string> thing = split(str,':');
	thing.at(1) = id;
	thing.at(2) = atkInd;

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
		//if (120 - p.party[p.leader].getAttackAt(move.attackIndex).getDamage() > std::rand() % 100)
		if(120-getPlayerAtIndex(move.client_id)->party[getPlayerAtIndex(move.client_id)->leader].getAttackAt(move.attackIndex).getDamage() > std::rand() % 100)
			server->sendToAllClients(serializeBattleEvent(move));
		else
		{
			BattleEvent newEvent;
			newEvent.client_id = 9;
			newEvent.attackIndex = 9;
			server->sendToAllClients(serializeBattleEvent(newEvent));
		}
	}
}

GameManager::GameManager()
{
	connected_players = std::vector<Player>();
	current_round = 0;
	event_queue = std::vector<BattleEvent>();
	deltaTime = 0;
	recved_events_from_this_phase = std::vector<int>();
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
		if (event_queue.size() >= 2)
		{
			std::cout << "Server: All players have sent their attacks. Broadcasting attacks and changing to DISPLAY_ATTACKS.\n";
			// broadcast the changes to the clients
			broadcastEventsToClients();
			event_queue.clear(); // clear the queue
			deltaTime = 0;

			setState(State::DISPLAY_ATTACKS);
			std::string msg = "CHANGESTATE DISPLAY_ATTACKS";
			server->sendToAllClients(msg.c_str());
		}
		break;
	case DISPLAY_ATTACKS:
		// wait like 10 seconds before changing back to choose attacks
		deltaTime += dt;
		std::cout << "Waiting for " << deltaTime << "s \n";
		if (deltaTime > 10.0f)
		{
			//tell the servers to choose attacks again
			std::string msg = "CHANGESTATE CHOOSE_ATTACKS";
			server->sendToAllClients(msg.c_str());
		}
		std::cout << "CHOOSE ATTACKS SHOULD BE CALLED HERE\n";
		break;
	}

	// check if anybody has won/lost
	if (int i = checkLoss() != -1)
	{
		//index at i lost
	}

}

bool GameManager::isReady()
{
	return (connected_players.size() == 2);
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
	std::cout << "Attack selected by " << battleEvent.client_id << ". AttackIndex: " << battleEvent.attackIndex << '\n';
	//GetGameManager().broadcastEventsToClients();
	std::string msg = "" + std::to_string(battleEvent.client_id) + " " + serializeBattleEvent(battleEvent);
	GetGameManager().local_client->sendToServer(msg);
}

// CALLED BY SERVER
	// called whenever the server receives a connection
void GameManager::trackPlayer(Player player)
{
	if (connected_players.size() >= 2) return; // stop tracking after 2
	connected_players.push_back(player);
	std::cout << "Tracked players: " << connected_players.size() << "\n";
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
void GameManager::queueEvent(const char* serializedBattleEvent)
{
	BattleEvent battleEvent = deserializeBattleEvent(serializedBattleEvent);
	//if we've already received an event from this player, skip it
	if (std::find(recved_events_from_this_phase.begin(), recved_events_from_this_phase.end(), battleEvent.client_id) != recved_events_from_this_phase.end()) {
		/* v contains x */
		return;
	}
	else {
		/* v does not contain x */
		std::cout << "Server: Queueing event from client: " << battleEvent.client_id << " using attack " << battleEvent.attackIndex << '\n';
		event_queue.push_back(battleEvent);
		recved_events_from_this_phase.push_back(battleEvent.client_id);
	}
}

// CALLED BY SERVER
void GameManager::broadcastEventsToClients()
{
	recved_events_from_this_phase.clear();

	for (BattleEvent move : event_queue)
	{
		// ask the server to send the msg to all clients
		server->sendToAllClients(serializeBattleEvent(move));
	}
	evaluateRound();
}

// CALLED BY CLIENTS
// when they receive the packet for a pokemon to update
void GameManager::updateEntry(int ownerId, int pokemonIndex, int damage)
{
	//apply damage to the other pokemon 
	getOtherPlayer(ownerId)->party[pokemonIndex].applyDamage(damage);

}

Player* GameManager::getPlayerAtIndex(size_t index)
{
	return &connected_players[index];
}

Player* GameManager::getOtherPlayer(size_t index)
{
	if (index == 0)
		index = 1;
	else
		index = 0;

	return &connected_players[index];
}

int GameManager::getLocalClientId()
{
	return local_client->getId();
}