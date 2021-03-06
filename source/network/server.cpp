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

#include "network/server.h"

#include <enetpp/server_listen_params.h>

#include "physics/physicsEngine.h"

// ~30 times a second
#define SERVER_TIME 0.03333

U32 Server::sUniqueId = 0;
U32 Server::sLastGhostId = 0;

Server::Server(World *world) : mWorld(world) {
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

	for (const auto &object : mWorld->getObjectList()) {
		auto netObject = dynamic_cast<NetObject*>(object);
		if (netObject != nullptr)
			addGhostedObject(netObject);
	}

	enetpp::server_listen_params<ClientConnection> params;
	params.set_max_client_count(16);
	params.set_channel_count(1);
	params.set_listen_port(28000);
	params.set_initialize_client_function([this](ClientConnection &client, const char *ipAddress) {
		// Initializes the client by assining its IP address and its unique identifier.
		client.mIpAddress = ipAddress;
		client.mId = sUniqueId;
		client.mServer = this;
		sUniqueId++;
	});
	mServer.start_listening(params);

	mIsRunning = true;
	mServerThread = std::thread(&Server::run, this);
}

void Server::stop() {
	// block this thread and wait for the server thread to finish before moving on
	mIsRunning = false;
	mServerThread.join();
	mServer.stop_listening();
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

		mWorld->loop(delta);

		// track time and sleep for a little bit so we don't kill our CPU
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		mTimer->end();
		delta = mTimer->getDelta();
	}
}

void Server::pollEvents() {
	auto onClientConnect = [this](ClientConnection &client) {
		this->onClientConnected(client);
	};

	auto onClientDisconnect = [](U32 clientId) {
		IO::printf("Client %u has left the server.\n", clientId);
	};

	auto onReceiveData = [this](ClientConnection &client, const U8 *data, size_t size) {
		this->onReceivePacket(client, data, size);
	};

	mServer.consume_events(onClientConnect, onClientDisconnect, onReceiveData);

	for (const auto &pair : mGhostedObjects) {
		const auto obj = pair.second;
		auto event = std::make_shared<NetServerGhostUpdateEvent>(this, nullptr, obj);

		//Don't send updates for people's control objects
		sendEvent(event, [this, obj](const ClientConnection &connection) {
			return obj != connection.getControlObject();
		}, ENetPacketFlag::ENET_PACKET_FLAG_UNSEQUENCED);
	}
}

void Server::onClientConnected(ClientConnection &client) {
	IO::printf("Client %u with IP %s has joined the server.\n", client.get_id(), client.getAddress().c_str());
	ghostAllObjects(client);
	client.createPlayer();
}

void Server::onReceivePacket(ClientConnection &client, const U8 *data, size_t size) {
	CharStream stream(data, size);
	NetServerEvent::deserialize(stream, this, &client);
}

void Server::sendEvent(const std::shared_ptr<NetServerEvent> &event, ENetPacketFlag flag) {
	const std::vector<U8> &data = event->serialize().getBuffer();
	mServer.send_packet_to_all_if(0, &data[0], data.size(), flag, [](const ClientConnection &){return true;});
}

void Server::sendEvent(const std::shared_ptr<NetServerEvent> &event, ClientConnection &connection, ENetPacketFlag flag) {
	const std::vector<U8> &data = event->serialize().getBuffer();
	mServer.send_packet_to(connection.get_id(), 0, &data[0], data.size(), flag);
}

void Server::sendEvent(const std::shared_ptr<NetServerEvent> &event, std::function<bool(const ClientConnection &)> predicate, ENetPacketFlag flag) {
	const std::vector<U8> &data = event->serialize().getBuffer();
	mServer.send_packet_to_all_if(0, &data[0], data.size(), flag, predicate);
}

void Server::addGhostedObject(NetObject *object) {
	//Get the object a ghosted id if we haven't seen it before
	if (mGhostedIndices.find(object) == mGhostedIndices.end()) {
		U32 index = sLastGhostId ++;
		mGhostedIndices[object] = index;
		mGhostedObjects[index] = object;
	}
}

void Server::ghostObject(NetObject *object) {
	addGhostedObject(object);

	//Create a ghosting packet
	auto event = std::make_shared<NetServerGhostCreateEvent>(this, nullptr, object);
	sendEvent(event);
}

void Server::ghostObject(NetObject *object, ClientConnection &connection) {
	addGhostedObject(object);

	//Create a ghosting packet
	auto event = std::make_shared<NetServerGhostCreateEvent>(this, &connection, object);
	sendEvent(event, connection);
}

void Server::ghostAllObjects() {
	//Ghost all objects to everyone
	for (const auto &pair : mGhostedObjects) {
		ghostObject(pair.second);
	}
}

void Server::ghostAllObjects(ClientConnection &connection) {
	//Ghost all objects to the client
	for (const auto &pair : mGhostedObjects) {
		ghostObject(pair.second, connection);
	}
}
