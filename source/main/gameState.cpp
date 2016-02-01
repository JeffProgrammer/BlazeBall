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
		ScriptEngine *scripting = new ScriptEngine();
		scripting->init("mainServer.chai");

		serverWorld = new World(platform->createPhysicsEngine(), scripting);
		server = new Server(serverWorld);
		serverWorld->loadLevel("bowl.json");
		server->start();
	}

	if (runClient) {
		ScriptEngine *scripting = new ScriptEngine();
		scripting->init("mainClient.chai");

		//Create us a new scene
		RenderWorld *world = new RenderWorld(platform->createPhysicsEngine(), scripting);
		client = new Client(world, serverAddress, 28000);

		world->setClient(client);
		client->connect();

		//Init SDL
		world->mConfig = new Config("config.txt");

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
