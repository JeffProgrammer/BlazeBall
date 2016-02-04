//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/client.h"

#include <enetpp/client_connect_params.h>
#include <Rocket/Controls/Controls.h>

#include "base/io.h"
#include "main/gameState.h"
#include "gui/elements/elementWorldView.h"

Client::Client(World *world, const std::string &ipAddress, const U16 port) : mWorld(world) {
	mServerAddress = ipAddress;
	mPort = port;

	mControlObject = nullptr;
}

Client::~Client() {
	delete mRenderer;
	Rocket::Core::Shutdown();
}

void Client::start() {
	mRenderer = new Renderer(this);
	mRenderer->setWindow(GameState::gState->platform->createWindow());

	//Load the base renderer and OpenGL
	if (!mRenderer->init()) {
		return;
	}

	// Initialize the gui library, librocket
	mGuiInterface = new GuiInterface();
	mGuiRenderInterface = new GuiRenderInterface(this, mRenderer->getWindow());
	Rocket::Core::SetSystemInterface(mGuiInterface);
	Rocket::Core::SetRenderInterface(mGuiRenderInterface);
	if (!Rocket::Core::Initialise()) {
		IO::printf("Unable to initialize rocket.\n");
		return;
	}

	//Custom Rocket controls
	ElementInstancerWorldView *instancerWorldView = new ElementInstancerWorldView(this);
	Rocket::Core::Factory::RegisterElementInstancer("WorldView", instancerWorldView);
	instancerWorldView->RemoveReference();

	Rocket::Controls::Initialise();

	//Load the GUI on the renderer
	if (!mRenderer->initGUI()) {
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

		updateMovement(lastDelta);
		mRenderer->render(lastDelta);

		timer->end();
		lastDelta = timer->getDelta();
	}
	delete timer;
}

void Client::stop() {
	mRunning = false;
}

void Client::updateMovement(const F64 &delta) {
	GameObject *control = getControlObject();
	if (control) {
		// temp code
		auto event = std::make_shared<NetClientGhostUpdateEvent>(this, control);
		sendEvent(event, ENetPacketFlag::ENET_PACKET_FLAG_UNSEQUENCED);

		if (control) {
			control->updateCamera(mMovement, delta);
			control->updateMove(mMovement, delta);
		}
	}

	mMovement.pitch = 0;
	mMovement.yaw = 0;

	if (mMovement.fire) {

	}
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