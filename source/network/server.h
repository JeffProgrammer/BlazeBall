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
#include <thread>
#include <enetpp/server.h>

class Server {
public:
	Server();
	~Server();

	void start();
	void stop();

	void pollEvents();

	void sendEvent(const NetServerEvent &event); //Global
	//TODO: Send to one client

private:
	static U32 sUniqueId;

	struct Connection {
		U32 id;
		std::string ipAddress;

		// This is needed for enet internally.
		U32 get_id() { return id; }
	};
	
	std::thread mServerThread;
	bool mIsRunning;
	PlatformTimer *mTimer;
	F64 mAccumulator;

	enetpp::server<Connection> mServer;

	void run();
};

#endif
