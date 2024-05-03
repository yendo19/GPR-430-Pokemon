#include "GameManager.h"

char* GameManager::serializeBattleEvent(BattleEvent battleEvent)
{
	// to be implemented
	std::string temp = "BATTLEEVENT:" + std::to_string(battleEvent.client_id) + ':' + std::to_string(battleEvent.attackIndex);
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
	connected_players = std::list<Player>();
	current_round = 0;
	event_queue = std::list<BattleEvent>();
}

GameManager::~GameManager()
{
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
void GameManager::updateEntry(int ownerId, int pokemonIndex, int damage)
{
	//apply damage to the other pokemon 
	getOtherPlayer(ownerId)->party[pokemonIndex].applyDamage(damage);

}

Player* GameManager::getPlayerAtIndex(size_t index)
{
	std::list<Player>::iterator nth = connected_players.begin();
	std::advance(nth, index-1);

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