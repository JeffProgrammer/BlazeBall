//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netClientEvent.h"
#include "network/client.h"
#include "base/io.h"
#include "game/gameObject.h"
#include "scriptEngine/abstractClassRep.h"

const U8 Magic = 0x42;

std::shared_ptr<NetClientEvent> NetClientEvent::deserialize(CharStream &data, Client *client) {
	//Don't corrupt the stream if it's not a net event
	if (data.peek<U8>() != NetEvent::Magic)
		return nullptr;
	//Ok, take off magic
	data.pop<U8>();

	Type type = static_cast<Type>(data.pop<U32>());
	std::shared_ptr<NetClientEvent> event;

	//TODO: Make this fancy
	switch (type) {
		case Event::NetConnect:
			event = std::make_shared<NetClientConnectEvent>(client);
			break;
		case Event::NetDisconnect:
			event = std::make_shared<NetClientDisconnectEvent>(client);
			break;
		case Event::NetGhostCreate:
			event = std::make_shared<NetClientGhostCreateEvent>(client);
			break;
		case Event::NetGhostUpdate:
			event = std::make_shared<NetClientGhostUpdateEvent>(client, nullptr);
			break;
		default:
			return nullptr;
	}

	event->read(data);
	return event;
}

bool NetClientEvent::write(CharStream &data) const {
	if (!NetEvent::write(data)) {
		return false;
	}

	//So we can identify as a NetClientEvent
	data.push<U8>(Magic);
	return true;
}

bool NetClientEvent::read(CharStream &data) {
	if (!NetEvent::read(data)) {
		return false;
	}

	//Make sure it's a NetClientEvent
	if (data.pop<U8>() != Magic)
		return false;

	//Nothing special here
	return true;
}

NetClientConnectEvent::NetClientConnectEvent(Client *client) : NetClientEvent(NetConnect, client) {
}

NetClientDisconnectEvent::NetClientDisconnectEvent(Client *client) : NetClientEvent(NetDisconnect, client) {
}

NetClientGhostCreateEvent::NetClientGhostCreateEvent(Client *client) : NetClientEvent(NetGhostCreate, client), mObject(nullptr) {
}

bool NetClientGhostCreateEvent::write(CharStream &data) const {
	//Don't try to create ghosted objects
	return false;
}

bool NetClientGhostCreateEvent::read(CharStream &data) {
	if (!NetClientEvent::read(data)) {
		return false;
	}

	U32 ghostId = data.pop<U32>();
	std::string className = data.pop<std::string>();

	mObject = dynamic_cast<NetObject *>(AbstractClassRep::createFromName(mClient->getWorld(), className));
	if (!mObject) {
		return false;
	}
	mClient->addGhostedObject(ghostId, mObject);

	if (!mObject->read(data)) {
		return false;
	}

	// This is a shitty way of doing this, but this is a level loader.
	// If the object is a game object, add it to the scene.
	GameObject *gameObject = dynamic_cast<GameObject *>(mObject);
	if (gameObject != nullptr) {
		// add it to the scene.
		mClient->getWorld()->addObject(gameObject);
	}

	return true;
}

NetClientGhostEvent::NetClientGhostEvent(Type type, Client *client, NetObject *object) : NetClientEvent(type, client), mObject(object) {
}

bool NetClientGhostEvent::write(CharStream &data) const {
	if (!NetClientEvent::write(data)) {
		return false;
	}

	//Can't exactly have a ghost event without an object
	if (!mObject)
		return false;

	//Push the ghost's id to the stream
	U32 ghostId = mClient->getGhostIndex(mObject);
	data.push<U32>(ghostId);

	return true;
}

bool NetClientGhostEvent::read(CharStream &data) {
	if (!NetClientEvent::read(data)) {
		return false;
	}

	//Find our object in the client
	U32 ghostId = data.pop<U32>();
	mObject = mClient->getGhostedObject(ghostId);

	//Doesn't exist?
	if (mObject == nullptr)
		return false;
	
	return true;
}


NetClientGhostUpdateEvent::NetClientGhostUpdateEvent(Client *client, NetObject *obj) : NetClientGhostEvent(NetGhostUpdate, client, obj) {
}

bool NetClientGhostUpdateEvent::write(CharStream &stream) const {
	if (!NetClientGhostEvent::write(stream))
		return false;

	//Write the object's data so that it can be sent to the server.
	if (!mObject->writeClientPacket(stream))
		return false;

	return true;
}

bool NetClientGhostUpdateEvent::read(CharStream &stream) {
	if (!NetClientGhostEvent::read(stream))
		return false;

	//Read the packet from the server.
	if (!mObject->readServerPacket(stream))
		return false;

	return true;
}