#pragma once
#include <list>
#include <vector>

class PokemonClient;
class PokemonServer;
struct Player;
struct BattleEvent;
enum State;

class GameManager
{
private:
	PokemonClient* local_client;
	PokemonServer* server;

	std::vector<Player> connected_players;

	// Round-related
	int current_round;
	std::vector<BattleEvent> event_queue;

	static char* serializeBattleEvent(BattleEvent battleEvent);
	static BattleEvent deserializeBattleEvent(const char* serialized_event);

	void evaluateRound();

	bool isServer;

	float deltaTime;
public:

	GameManager();
	~GameManager();

	bool isReady();

	static void acceptAttackInput(BattleEvent battleEvent);

	// CALLED BY SERVER
	// called whenever the server receives a connection
	void trackPlayer(Player player);

	// CALLED BY CLIENT
	void sendEventToServer(BattleEvent battleEvent);

	// CALLED BY SERVER
	// this should be called whenever the server receives an event
	void queueEvent(const char* serializedBattleEvent);

	// CALLED BY SERVER
	void broadcastEventsToClients();

	// CALLED BY CLIENTS
	// when they receive the packet for a pokemon to update
	void updateEntry(int ownerId,int pokemonIndex,int damage);

	int checkLoss();

	static GameManager & GetGameManager() {
		static GameManager gm;
		return gm;
	}

	Player* getPlayerAtIndex(size_t index);

	Player* getOtherPlayer(size_t index);

	bool getIsServer() { return isServer; }

	void setState(State state);
	
	void update(float dt, int frame_num);

	void setServer(PokemonServer* srv)
	{
		server = srv;
		isServer = true;
	}

	void setClient(PokemonClient* cli)
	{
		local_client = cli;
	}

	int getLocalClientId();
	
};