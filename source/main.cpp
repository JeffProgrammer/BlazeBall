//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include <cstdlib>
#include <memory>

#include "platform/SDL/SDLPlatform.h"
#include "game/GameInterior.h"
#include "scriptEngine/scriptEngine.h"
#include "game/world.h"
#include "render/renderWorld.h"

// TODO: clean this shit up
#include "physics/bullet/btPhysicsEngine.h"
#include "game/levelLoader.h"

#include "network/network.h"
#include "network/server.h"
#include "network/client.h"

extern GLuint gSphereVBO;

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
};

int main(int argc, const char **argv) {
	GameState state(std::make_unique<SDLPlatform>());

	// Load the networking engine
	Network::init();

	// Init script engine and call the main function
	if (!ScriptEngine::getSingleton()->init())
		return 1;

	// parse command line arguments.
	state.parseArgs(argc, argv);

	if (state.runServer) {
		World *world = new World(new btPhysicsEngine());
		Server *server = new Server(world);
		world->loadLevel("level.json");
		server->start();

		state.server = server;
		state.serverWorld = world;
	}

	if (state.runClient) {
		//Create us a new scene
		RenderWorld *world = new RenderWorld(new btPhysicsEngine());
		Client *client = new Client(world, state.serverAddress, 28000);

		world->setClient(client);
		client->connect();

		//Init SDL
		world->mWindow = state.platform->createWindow();
		world->mTimer = state.platform->createTimer();
		world->mConfig = new Config("config.txt");

		if (!world->init()) {
			return 1;
		}

		state.client = client;
		state.clientWorld = world;
	}

	F64 lastDelta = 0.0;

	PlatformTimer *timer = state.platform->createTimer();

	while (true) {
		//Profiling
		timer->start();
		{
			if (state.runClient) {
				state.clientWorld->loop(lastDelta);
				state.client->pollEvents();

				if (!state.clientWorld->getRunning()) {
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

	if (state.runClient) {
		state.client->disconnect();

		// much hack, very wow
		if (gSphereVBO)
			glDeleteBuffers(1, &gSphereVBO);
	}
	if (state.runServer) {
		state.server->stop();
	}

	// destroy the networking engine
	Network::destroy();

	return 0;
}

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