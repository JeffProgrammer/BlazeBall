//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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

bool GameState::start() {
	if (runServer) {
		// Init server scripting
		if (!ScriptEngine::get(ScriptEngine::SERVER)->init("mainServer.chai"))
			return false;

		serverWorld = new World(platform->createPhysicsEngine());
		server = new Server(serverWorld);
		serverWorld->loadLevel("level.json");
		server->start();
	}

	if (runClient) {
		// Init client scripting
		if (!ScriptEngine::get(ScriptEngine::CLIENT)->init("mainClient.chai"))
			return false;

		//Create us a new scene
		RenderWorld *world = new RenderWorld(platform->createPhysicsEngine());
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
