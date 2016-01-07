//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_SERVER_H_
#define _NETWORK_SERVER_H_

#include <thread>
#include <enetpp/server.h>

#include "base/types.h"
#include "base/io.h"
#include "platform/platformTimer.h"
#include "platform/SDL/SDLTimer.h"
#include "network/event/netServerEvent.h"
#include "network/clientConnection.h"
#include "game/gameObject.h"

class Server {
public:
	Server();
	~Server();

	void start();
	void stop();

	void pollEvents();

	void sendEvent(const std::shared_ptr<NetServerEvent> &event); //Global
	void sendEvent(const std::shared_ptr<NetServerEvent> &event, ClientConnection *connection);

	void ghostObject(NetObject *object); //Global
	//TODO: Individual ghosting

	NetObject *getGhostedObject(U32 index) {
		return mGhostedObjects[index];
	}

private:
	static U32 sUniqueId;
	static U32 sLastGhostId;

	std::thread mServerThread;
	bool mIsRunning;
	PlatformTimer *mTimer;
	F64 mAccumulator;

	std::map<U32, GameObject *> mGhostedObjects;

	enetpp::server<ClientConnection> mServer;

	void run();
};

#endif
