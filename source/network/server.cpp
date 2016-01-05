//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/server.h"

Server::Server() {
	mIsRunning = false;
}

Server::~Server() {

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
	while (mIsRunning) {
		IO::printf("Server is running.\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}