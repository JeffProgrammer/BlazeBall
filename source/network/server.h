//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
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
	Server(World *world);
	~Server();

	void start();
	void stop();

	void pollEvents();

	void sendEvent(const std::shared_ptr<NetServerEvent> &event, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE); //Global
	void sendEvent(const std::shared_ptr<NetServerEvent> &event, ClientConnection &connection, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
	void sendEvent(const std::shared_ptr<NetServerEvent> &event, std::function<bool(const ClientConnection &)> predicate, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);

	void onClientConnected(ClientConnection &client);
	void onReceivePacket(ClientConnection &client, const U8 *data, size_t size);

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

	World *getWorld() const {
		return mWorld;
	}

private:
	static U32 sUniqueId;
	static U32 sLastGhostId;

	std::thread mServerThread;
	bool mIsRunning;
	PlatformTimer *mTimer;
	F64 mAccumulator;
	World *mWorld;

	std::unordered_map<U32, NetObject *> mGhostedObjects;
	std::unordered_map<NetObject *, U32> mGhostedIndices;

	enetpp::server<ClientConnection> mServer;

	void run();
};

#endif
