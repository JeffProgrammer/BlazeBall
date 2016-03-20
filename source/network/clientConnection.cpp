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

#include "clientConnection.h"
#include "network/event/netServerEvent.h"
#include "network/server.h"

#include "game/camera.h"
#include "game/sphere.h"

void ClientConnection::setControlObject(GameObject *object) {
	mControlObject = object;

	//Update our client
	auto controlObjectEvent = std::make_shared<NetServerGhostControlObjectEvent>(mServer, this, object);
	mServer->sendEvent(controlObjectEvent, *this);

	auto updateEvent = std::make_shared<NetServerGhostUpdateEvent>(mServer, this, object);
	mServer->sendEvent(updateEvent, *this);
}

void ClientConnection::createPlayer() {
	//Create camera
	{
		Camera *camera = static_cast<Camera *>(AbstractClassRep::createFromName("Camera", mServer->getWorld()));
		mServer->getWorld()->addObject(camera);
		mServer->ghostObject(camera);
		mCamera = camera;
	}

	//Create player
	{
		Sphere *player = static_cast<Sphere *>(AbstractClassRep::createFromName("Sphere", mServer->getWorld()));
		player->setPosition(glm::vec3(0, 0, 20));
		player->setRadius(0.3f);
		player->setMass(1);
		mServer->getWorld()->addObject(player);
		mServer->ghostObject(player);
		mPlayer = player;
	}

	setControlObject(mPlayer);
}