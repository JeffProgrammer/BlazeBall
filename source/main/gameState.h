//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _MAIN_GAMESTATE_H_
#define _MAIN_GAMESTATE_H_

#include <memory>

#include "platform/SDL/SDLPlatform.h"
#include "game/world.h"
#include "network/server.h"
#include "network/client.h"

class GameState {
public:
	std::unique_ptr<Platform> platform;

	bool runClient;
	bool runServer;

	std::string serverAddress;

	Client *client;
	Server *server;

	World *clientWorld;
	World *serverWorld;

	GameState(std::unique_ptr<Platform> &&platform) {
		this->platform = std::move(platform);
		//Local server by default
		runClient = true;
		runServer = true;
		serverAddress = "localhost";
	}
	~GameState() {

	}

	void parseArgs(int argc, const char **argv);

	bool start();
	void stop();
	void runLoop();
};

#endif
