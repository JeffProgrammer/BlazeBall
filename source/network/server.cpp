//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/server.h"

#include <enetpp/server_listen_params.h>

#include "physics/physicsEngine.h"

// ~30 times a second
#define SERVER_TIME 0.03333

U32 Server::sUniqueId = 0;

Server::Server() {
	mIsRunning = false;
	mAccumulator = 0.0;
	mTimer = new SDLTimer();
}

Server::~Server() {
	delete mTimer;
}

void Server::start() {
	if (mIsRunning) {
		IO::printf("Server thread is already started.\n");
		return;
	}

	// start the server process
	IO::printf("Starting server process...\n");

	enetpp::server_listen_params<Connection> params;
	params.set_max_client_count(16);
	params.set_channel_count(1);
	params.set_listen_port(28000);
	params.set_initialize_client_function([](Connection &client, const char *ipAddress) {
		// Initializes the client by assining its IP address and its unique identifier.
		client.ipAddress = ipAddress;
		client.id = sUniqueId;
		sUniqueId++;
	});
	mServer.start_listening(params);

	mIsRunning = true;
	mServerThread = std::thread(&Server::run, this);
}

void Server::stop() {
	// block this thread and wait for the server thread to finish before moving on
	mServer.stop_listening();
	mIsRunning = false;
	mServerThread.join();
	IO::printf("Stopping server process...\n");
}

void Server::run() {
	F64 delta = 0.0;
	while (mIsRunning) {
		mTimer->start();

		// fire physics.
		// the physics engine handle's its own timestep.

		// network update
		mAccumulator += delta;
		while (mAccumulator > SERVER_TIME) {
			pollEvents();
			mAccumulator -= SERVER_TIME;
		}

		// track time and sleep for a little bit so we don't kill our CPU
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		mTimer->end();
		delta = mTimer->getDelta();
	}
}

void Server::pollEvents() {
	auto onClientConnect = [](Connection &client) {
		IO::printf("Client %u with IP %s has joined the server.\n", client.id, client.ipAddress.c_str());
	};

	auto onClientDisconnect = [](U32 clientId) {
		IO::printf("Client %u has left the server.\n", clientId);
	};

	auto onReceiveData = [this](Connection &client, const U8 *data, size_t size) {
		CharStream stream(data, size);
		NetServerEvent *event = NetServerEvent::deserialize(stream, this);
	};

	mServer.consume_events(onClientConnect, onClientDisconnect, onReceiveData);
}