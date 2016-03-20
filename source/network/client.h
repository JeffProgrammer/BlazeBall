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

#ifndef _NETWORK_CLIENT_H_
#define _NETWORK_CLIENT_H_

#include <unordered_map>
#include <enetpp/client.h>
#include <Rocket/Core.h>
#include <Rocket/Core/Input.h>
#include <Rocket/Core/ElementDocument.h>

#include "base/types.h"
#include "network/event/netClientEvent.h"
#include "network/netObject.h"
#include "game/world.h"
#include "game/sphere.h"
#include "platform/platformTimer.h"
#include "render/renderer.h"
#include "game/movement.h"
#include "gui/renderInterface.h"
#include "gui/systemInterface.h"

class Client {
public:
	Client(World *world, const std::string &serverAddress, const U16 port);
	~Client();

	void start();
	void stop();

	void connect();
	void disconnect();

	void pollEvents();
	void updateMovement(const F64 &delta);

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

	//Read-only variables
	World *getWorld() { return mWorld; }
	Renderer *getRenderer() { return mRenderer; }
	Movement &getMovement() { return mMovement; }

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
	Movement mMovement;

	GuiInterface *mGuiInterface;
	GuiRenderInterface *mGuiRenderInterface;

	std::unordered_map<U32, NetObject *> mGhostedObjects;
	std::unordered_map<NetObject *, U32> mGhostedIndices;
};

#endif // _NETWORK_CLIENT_H_
