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

#endif // _NETWORK_SERVER_H_