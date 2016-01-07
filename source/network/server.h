//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_SERVER_H_
#define _NETWORK_SERVER_H_

#include "base/types.h"
#include "base/io.h"
#include "platform/platformTimer.h"
#include "platform/SDL/SDLTimer.h"
#include "network/event/netServerEvent.h"
#include "network/clientConnection.h"
#include <thread>
#include <enetpp/server.h>

class Server {
public:
	Server();
	~Server();

	void start();
	void stop();

	void pollEvents();

	void sendEvent(const std::shared_ptr<NetServerEvent> &event); //Global
	void sendEvent(const std::shared_ptr<NetServerEvent> &event, ClientConnection *connection);

private:
	static U32 sUniqueId;

	std::thread mServerThread;
	bool mIsRunning;
	PlatformTimer *mTimer;
	F64 mAccumulator;

	enetpp::server<ClientConnection> mServer;

	void run();
};

#endif
