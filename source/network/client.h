//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_CLIENT_H_
#define _NETWORK_CLIENT_H_

#include <unordered_map>
#include <enetpp/client.h>

#include "base/types.h"
#include "network/event/netClientEvent.h"
#include "network/netObject.h"
#include "game/world.h"
#include "game/sphere.h"
#include "platform/platformTimer.h"
#include "render/renderer.h"

class Client {
public:
	Client(World *world, const std::string &serverAddress, const U16 port);
	~Client();

	void start();
	void stop();

	void connect();
	void disconnect();

	void pollEvents();

	void sendEvent(const std::shared_ptr<NetClientEvent> &event, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
	void onReceivePacket(const U8 *data, size_t size);

	NetObject *getGhostedObject(U32 index) {
		return mGhostedObjects[index];
	}
	U32 getGhostIndex(NetObject *object) {
		return mGhostedIndices[object];
	}
	void addGhostedObject(U32 index, NetObject *object) {
		mGhostedObjects[index] = object;
		mGhostedIndices[object] = index;
	}
	World *getWorld() {
		return mWorld;
	}

	GameObject *getControlObject() const { return mControlObject; }
	void setControlObject(GameObject *object) { mControlObject = object; dynamic_cast<Sphere *>(object)->setActivationState(true); }

protected:
	enetpp::client mClient;
	std::string mServerAddress;
	U16 mPort;
	World *mWorld;
	GameObject *mControlObject;
	Renderer *mRenderer;
	bool mRunning;

	std::unordered_map<U32, NetObject *> mGhostedObjects;
	std::unordered_map<NetObject *, U32> mGhostedIndices;
};

#endif
