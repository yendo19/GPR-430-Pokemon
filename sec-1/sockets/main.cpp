#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>

#include "socklib.h"
#include "defer.h"
#include "PokemonCreation/MyParty.h"

myParty myInv;

int run_server() {
	// Simple demo to demonstrate serialization
	// over TCP
	// Create a socket, wait for folks to connect
	Socket listen_sock(Socket::Family::INET, Socket::Type::STREAM);
	listen_sock.Bind(Address("0.0.0.0", 36925));
	listen_sock.Listen();

	while (true) {
		std::cout << "server Running";
	}
}

int main()
{
	std::cout << "hi";
	myInv.Init();
}

//int main(int argc, char *argv[]) {
//
//	std::cout << "hi";
//
//	myInv.Init();

	//SockLibInit();
	//atexit(SockLibShutdown);

	//if (argc > 1) {
	//	return run_server();
	//}

	//// Initialize the system
	//float last_frame = now();
	//bool quit = false;
	//int frame_num = 0;
	//// One second per frame -- wow, that's slow!
	//const float targetDt = 1;

	//// Initialize the player "object"
	//float character_pos = 0;
	//float xVelocity = 1;

	//// Initialize the socket system
	//NetworkModule netsys;
	//netsys.Init();

	//GameObject go;
	//go.x = 100001;
	//go.y = 100002;
	//go.z = 100003;
	//go.xVel = 100010;
	//go.yVel = 100011;
	//go.zVel = 100012;

	//go.sprite = "Hello, there!";
	//std::cout << "===== Game Object as Bytes =====\n";
	//print_as_bytes((char*)&go, sizeof(go));

	//{
	//	char buffer[4096];
	//	size_t amt_written = SerializeGameObjectAsString(&go, buffer, sizeof(buffer));
	//	std::string go_str(buffer, amt_written);
	//	std::cout << "===== Game Object as String =====\n";
	//	std::cout << "===== Size: " << amt_written << "   ====\n";
	//	std::cout << "\"" << go_str << "\"" << std::endl;
	//}

	//{
	//	char buffer[4096] = { 0 };
	//	size_t amt_written = SerializeGameObjectAsBytes(&go, buffer, sizeof(buffer));
	//	std::cout << "==== Serialized game object as Bytes =====\n";
	//	std::cout << "===== Size: " << amt_written << "   ====\n";
	//	print_as_bytes(buffer, amt_written);
	//}

	//return 0;

	//while (!quit) {
	//	float time = now();
	//	float dt = time - last_frame;
	//	if (dt < targetDt)
	//		continue;
	//	frame_num++;

	//	// Process input -- is the keyboard pressed,
	//	// is the mouse down, etc.?
	//	netsys.Update(dt, frame_num);
	//	
	//	// Update all the game objects
	//	character_pos += xVelocity * dt;

	//	// Render
	//	// Clear the back buffer
	//	system("cls");

	//	for (int i = 0; i < character_pos; i++) {
	//		std::cout << " ";
	//	}
	//	std::cout << "@";
	//	std::cout << std::endl << std::endl;
	//	std::cout << to_display;

	//	to_display = "";
	//	last_frame = time;
	//}


	//return 0;
//
