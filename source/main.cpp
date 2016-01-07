//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include <cstdlib>
#include "platform/SDL/SDLWindow.h"
#include "platform/SDL/SDLTimer.h"
#include "platform/network.h"
#include "game/GameInterior.h"
#include "scriptEngine/scriptEngine.h"
#include "game/world.h"
#include "render/renderWorld.h"

// TODO: clean this shit up
#include "physics/bullet/btPhysicsEngine.h"
#include "game/levelLoader.h"
#include "network/server.h"
#include "network/client.h"

extern GLuint gSphereVBO;

bool gIsDedicated = false;

void parseArgs(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {

	// Load the networking engine
	Network::init();

	// Init script engine and call the main function
	if (!ScriptEngine::getSingleton()->init())
		return 1;

	// parse command line arguments.
	parseArgs(argc, argv);

	if (gIsDedicated) {
		// create a server obj
		Server server;
		server.start();

		// block until we quit
		std::string line;
		while (true) {  
			std::getline(std::cin, line);
			if (line == "quit")
				break;
		}

		// stop server
		server.stop();
	} else {
		//Create us a new scene
		RenderWorld *world = new RenderWorld(new btPhysicsEngine());

		// NETWORKING MWHAHAHAHAH
		Client client(world, "127.0.0.1", 28000);
		client.connect();

		//Init SDL
		world->mWindow = new SDLWindow();
		world->mTimer = new SDLTimer();
		world->mConfig = new Config("config.txt");

		world->loadLevel("level.json");

		client.disconnect();

		// much hack, very wow
		if (gSphereVBO)
			glDeleteBuffers(1, &gSphereVBO);
	}


	// destroy the networking engine
	Network::destroy();

	return 0;
}

void parseArgs(int argc, const char *argv[]) {
	for (int i = 1; i < argc; i++) {
		std::string cmp = argv[i];
		if (cmp == "-dedicated")
			gIsDedicated = true;
	}
}