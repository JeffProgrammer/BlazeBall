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

	bool start();
	void stop();
	void runLoop();
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

	if (!state.start())
		return 1;

	state.runLoop();
	state.stop();

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

bool GameState::start() {
	if (runServer) {
		serverWorld = new World(new btPhysicsEngine());
		server = new Server(serverWorld);
		serverWorld->loadLevel("level.json");
		server->start();
	}

	if (runClient) {
		//Create us a new scene
		RenderWorld *world = new RenderWorld(new btPhysicsEngine());
		client = new Client(world, serverAddress, 28000);

		world->setClient(client);
		client->connect();

		//Init SDL
		world->mWindow = platform->createWindow();
		world->mTimer = platform->createTimer();
		world->mConfig = new Config("config.txt");

		if (!world->init()) {
			return false;
		}

		clientWorld = world;
	}
	return true;
}

void GameState::stop() {
	if (runClient) {
		client->disconnect();

		// much hack, very wow
		if (gSphereVBO)
			glDeleteBuffers(1, &gSphereVBO);
	}
	if (runServer) {
		server->stop();
	}
}

void GameState::runLoop() {
	F64 lastDelta = 0.0;
	PlatformTimer *timer = platform->createTimer();

	while (true) {
		//Profiling
		timer->start();
		{
			if (runClient) {
				clientWorld->loop(lastDelta);
				client->pollEvents();

				if (!clientWorld->getRunning()) {
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
}
