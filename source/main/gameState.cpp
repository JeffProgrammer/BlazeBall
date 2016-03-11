//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameState.h"

#include "render/renderWorld.h"

extern GLuint gSphereVBO;

void GameState::parseArgs(int argc, const char **argv) {
	for (int i = 1; i < argc; i ++) {
		std::string cmp = argv[i];
		if (cmp == "--dedicated") {
			runClient = false;
			runServer = true;
		}
		if (cmp == "--server" && argc > (i + 1)) {
			runClient = true;
			runServer = false;
			serverAddress = argv[i + 1];
		}
	}
}

void GameState::createServer(U16 port) {
	serverWorld = new World(new PhysicsEngine());
	server = new Server(serverWorld, port);
	server->start();
}

void GameState::connectToServer(const std::string &address, U16 port) {
	client->connect(address, port);
}

void GameState::disconnectFromServer() {
	client->disconnect();
}

bool GameState::start() {
	if (runClient) {
		client = new Client();
	}
	return true;
}

void GameState::stop() {
	if (runClient) {
		client->disconnect();

		// much hack, very wow
		if (gSphereVBO) {
			glDeleteBuffers(1, &gSphereVBO);
		}
		delete client;
		delete clientWorld;
	}
	if (runServer) {
		server->stop();
		delete server;
		delete serverWorld;
	}
}

void GameState::runLoop() {
	//TODO server quit events
	client->start();
}
