//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_SERVER_H_
#define _NETWORK_SERVER_H_

#include <thread>
#include <unordered_map>
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

	void sendEvent(const std::shared_ptr<NetServerEvent> &event, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE); //Global
	void sendEvent(const std::shared_ptr<NetServerEvent> &event, ClientConnection &connection, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);

	void addGhostedObject(NetObject *object);
	void ghostObject(NetObject *object); //Global
	void ghostObject(NetObject *object, ClientConnection &connection);

	void ghostAllObjects(); //Global
	void ghostAllObjects(ClientConnection &connection);

	NetObject *getGhostedObject(U32 index) {
		return mGhostedObjects[index];
	}
	U32 getGhostIndex(NetObject *object) {
		return mGhostedIndices[object];
	}

private:
	static U32 sUniqueId;
	static U32 sLastGhostId;

	std::thread mServerThread;
	bool mIsRunning;
	PlatformTimer *mTimer;
	F64 mAccumulator;

	std::unordered_map<U32, NetObject *> mGhostedObjects;
	std::unordered_map<NetObject *, U32> mGhostedIndices;

	enetpp::server<ClientConnection> mServer;

	void run();
};

#endif
