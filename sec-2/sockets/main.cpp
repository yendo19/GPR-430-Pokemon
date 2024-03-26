#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>

#include "socklib.h"
#include "defer.h"

float clocks_to_secs(clock_t clocks) {
	return (float)clocks / CLOCKS_PER_SEC;
}

float time_now() {
	return clocks_to_secs(clock());
}

int main(int argc, char *argv[]) {

	// Game loop structure
	// ===================
	// 
	//	While the game is running:
	//		If enough time has elapsed:
	//			Poll inputs
	//			Update all game objects
	//			Render game to the screen

	// Initialize system variables
	bool quit = false;
	float last_frame_time = time_now();
	float targetDt = 1;
	std::string msg_to_display;

	// Initialize starting game objects
	float character_pos = 0;
	float xVelocity = 1;

	// Initialize networking module
	SockLibInit();
	defer _([]() {SockLibShutdown();});
	Socket sock(Socket::Family::INET, Socket::Type::STREAM);
	sock.Connect(Address("68.183.63.165", 7778));
	sock.SetNonBlockingMode(true);
	char message_buffer[4096];

	// Run game loop
	while (!quit) {
		float now = time_now();
		float dt = now - last_frame_time;
		if (dt < targetDt)
			continue;

		// Poll inputs
		if (rand() % 2 == 0) {
			char buffer[64];
			size_t strsize = 0;
			strsize += snprintf(buffer, sizeof(buffer), "LIST");
			for (int i = 0; i < 5; i++) {
				strsize += snprintf(buffer + strsize,
					sizeof(buffer) - strsize,
					" %i", rand() % 500);
			}

			/*
			std::stringstream ss;
			ss << "LIST";
			for (int i = 0; i < 5; i++) {
				ss << " " << rand() % 500;
			}
			*/
			sock.Send(buffer, strsize);
		}

		int nbytes_recvd = sock.Recv(message_buffer, sizeof(message_buffer));
		if (nbytes_recvd == -1) {
			if (sock.GetLastError() == Socket::SOCKLIB_EWOULDBLOCK) {
				msg_to_display = "No message this frame.\n";
			}
			else {
				perror("recv()");
				abort();
			}
		}
		else if (nbytes_recvd == 0) {
			std::cerr << "Connection hung up unexpectedly!\n";
			abort();
		}
		else {
			msg_to_display = std::string(message_buffer, nbytes_recvd);
		}

		// Update all game objects
		character_pos += xVelocity * dt;

		// Render game to the screen
		// Clear back buffer
		system("cls");
		// Draw to back buffer
		for (int i = 0; i < character_pos; i++) {
			std::cout << " ";
		}
		std::cout << "@";
		std::cout << "\n\n" << msg_to_display;

		last_frame_time = now;
	}

	return 0;
}
