//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
		Camera *camera = static_cast<Camera *>(AbstractClassRep::createFromName(mServer->getWorld(), "Camera"));
		mServer->getWorld()->addObject(camera);
		mServer->ghostObject(camera);
		mCamera = camera;
	}

	//Create player
	{
		Sphere *player = static_cast<Sphere *>(AbstractClassRep::createFromName(mServer->getWorld(), "Sphere"));
		player->setPosition(glm::vec3(0, 0, 20));
		player->setRadius(0.675);
		player->setMass(1);
		mServer->getWorld()->addObject(player);
		mServer->ghostObject(player);
		mPlayer = player;
	}

	setControlObject(mPlayer);
}