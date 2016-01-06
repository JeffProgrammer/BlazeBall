//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/server.h"
#include "physics/physicsEngine.h"

// ~30 times a second
#define SERVER_TIME 0.03333

Server::Server() {
	mIsRunning = false;
	mAccumulator = 0.0;
	mTimer = new SDLTimer;
}

Server::~Server() {
	delete mTimer;
}

void Server::start() {
	if (mIsRunning) {
		IO::printf("Server thread is already started.\n");
		return;
	}

	// start the server process
	IO::printf("Starting server process...\n");
	mIsRunning = true;
	mServerThread = std::thread(&Server::run, this);
}

void Server::stop() {
	// block this thread and wait for the server thread to finish before moving on
	mIsRunning = false;
	mServerThread.join();
	IO::printf("Stopping server process...\n");
}

void Server::run() {
	F64 delta = 0.0;
	while (mIsRunning) {
		mTimer->start();

		// fire physics.
		// the physics engine handle's its own timestep.

		// network update
		mAccumulator += delta;
		while (mAccumulator > SERVER_TIME) {
			handleNetwork();
			mAccumulator -= SERVER_TIME;
		}

		// track time and sleep for a little bit so we don't kill our CPU
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		mTimer->end();
		delta = mTimer->getDelta();
	}
}

void Server::handleNetwork() {

}