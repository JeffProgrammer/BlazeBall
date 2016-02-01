//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/client.h"
#include "base/io.h"
#include "main/gameState.h"
#include <enetpp/client_connect_params.h>

Client::Client(World *world, const std::string &ipAddress, const U16 port) : mWorld(world) {
	mServerAddress = ipAddress;
	mPort = port;

	mControlObject = nullptr;
}

Client::~Client() {
	delete mRenderer;
}

void Client::start() {
	mRenderer = new Renderer(this);
	mRenderer->setWindow(GameState::gState->platform->createWindow());
	if (!mRenderer->init()) {
		return;
	}

	mRunning = true;

	PlatformTimer *timer = GameState::gState->platform->createTimer();
	F64 lastDelta = 0.0;
	while (mRunning) {
		timer->start();

		mWorld->loop(lastDelta);
		pollEvents();

		if (!mWorld->getRunning()) {
			stop();
			return;
		}

		mRenderer->render(lastDelta);

		timer->end();
		lastDelta = timer->getDelta();
	}
	delete timer;
}

void Client::stop() {
	mRunning = false;
}

void Client::connect() {
	IO::printf("Connecting to IP address: %s\n", mServerAddress.c_str());

	enetpp::client_connect_params params;
	params.set_channel_count(1);
	params.set_server_host_name_and_port(mServerAddress.c_str(), mPort);

	mClient.connect(params);
}

void Client::disconnect() {
	IO::printf("Disconnecting!");
	mClient.disconnect();
}

void Client::pollEvents() {
	auto onConnect = [this]() {
		IO::printf("You have connected to the server!\n");
		this->sendEvent(std::make_shared<NetClientConnectEvent>(this));
	};

	auto onDisconnect = []() {
		IO::printf("You have disconnected from the server!\n");
	};

	auto onReceiveData = [this](const U8 *data, size_t size) {
		this->onReceivePacket(data, size);
	};

	mClient.consume_events(onConnect, onDisconnect, onReceiveData);
}

void Client::onReceivePacket(const U8 *data, size_t size) {
	CharStream stream(data, size);
	NetClientEvent::deserialize(stream, this);
}

void Client::sendEvent(const std::shared_ptr<NetClientEvent> &event, ENetPacketFlag flag) {
	const std::vector<U8> &data = event->serialize().getBuffer();

	mClient.send_packet(0, reinterpret_cast<const U8 *>(&data[0]), data.size(), flag);
}