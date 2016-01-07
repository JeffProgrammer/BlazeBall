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

struct GameState {
	bool runClient;
	bool runServer;

	std::string serverAddress;

	Client *client;
	Server *server;

	World *clientWorld;
	World *serverWorld;

	GameState() {
		//Local server by default
		runClient = true;
		runServer = true;
		serverAddress = "127.0.0.1";
	}
} gConnectionState;

void parseArgs(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {
	// Load the networking engine
	Network::init();

	// Init script engine and call the main function
	if (!ScriptEngine::getSingleton()->init())
		return 1;

	// parse command line arguments.
	parseArgs(argc, argv);

	if (gConnectionState.runServer) {
		World *world = new World(new btPhysicsEngine());
		Server *server = new Server(world);
		world->loadLevel("level.json");
		server->start();

		gConnectionState.server = server;
		gConnectionState.serverWorld = world;
	}

	if (gConnectionState.runClient) {
		//Create us a new scene
		RenderWorld *world = new RenderWorld(new btPhysicsEngine());
		Client *client = new Client(world, gConnectionState.serverAddress, 28000);
		client->connect();

		//Init SDL
		world->mWindow = new SDLWindow();
		world->mTimer = new SDLTimer();
		world->mConfig = new Config("config.txt");

		if (!world->init()) {
			return 1;
		}

		gConnectionState.client = client;
		gConnectionState.clientWorld = world;
	}

	F64 lastDelta = 0.0;

	SDLTimer *timer = new SDLTimer();

	while (true) {
		//Profiling
		timer->start();
		{
			if (gConnectionState.runClient) {
				gConnectionState.clientWorld->loop(lastDelta);
				gConnectionState.client->pollEvents();

				if (!gConnectionState.clientWorld->getRunning()) {
					break;
				}
			} else {
				std::string input;
				std::getline(std::cin, input);
				if (input == "quit")
					break;
			}
		}
		timer->end();
		lastDelta = timer->getDelta();
	}

	if (gConnectionState.runClient) {
		gConnectionState.client->disconnect();

		// much hack, very wow
		if (gSphereVBO)
			glDeleteBuffers(1, &gSphereVBO);
	}
	if (gConnectionState.runServer) {
		gConnectionState.server->stop();
	}

	// destroy the networking engine
	Network::destroy();

	return 0;
}

void parseArgs(int argc, const char *argv[]) {
	for (int i = 1; i < argc; i++) {
		std::string cmp = argv[i];
		if (cmp == "--dedicated") {
			gConnectionState.runClient = false;
			gConnectionState.runServer = true;
		}
		if (cmp == "--server" && argc > i + 1) {
			gConnectionState.runClient = true;
			gConnectionState.runServer = false;
			gConnectionState.serverAddress = argv[i + 1];
		}
	}
}