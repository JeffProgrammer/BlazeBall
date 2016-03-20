//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
		serverWorld = new World(new PhysicsEngine());
		server = new Server(serverWorld);
		serverWorld->loadLevel("bowl.json");
		server->start();
	}

	if (runClient) {
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
