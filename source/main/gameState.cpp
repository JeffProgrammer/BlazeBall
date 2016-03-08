//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameState.h"

#include "render/renderWorld.h"
#include "resource/resourceLoader.h"

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
		serverWorld = new World(new PhysicsEngine());
		server = new Server(serverWorld);
		serverWorld->loadLevel("bowl.json");
		server->start();
	}

	if (runClient) {
		// init resource loader
		ResourceLoader::create();

		//Create us a new scene
		RenderWorld *world = new RenderWorld(new PhysicsEngine());
		client = new Client(world, serverAddress, 28000);

		world->setClient(client);
		client->connect();

		clientWorld = world;
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

		// cleanup resource loader
		ResourceLoader::destroy();

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
