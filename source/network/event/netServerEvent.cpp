//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netServerEvent.h"
#include "network/server.h"
#include "base/io.h"

const U8 Magic = 0x42;

std::shared_ptr<NetServerEvent> NetServerEvent::deserialize(CharStream &data, Server *server, ClientConnection *client) {
	//Don't corrupt the stream if it's not a net event
	if (data.peek<U8>() != NetEvent::Magic)
		return nullptr;
	//Ok, take off magic
	data.pop<U8>();

	Type type = static_cast<Type>(data.pop<U32>());
	std::shared_ptr<NetServerEvent> event;

	//TODO: Make this fancy
	switch (type) {
		case Event::NetConnect:
			event = std::make_shared<NetServerConnectEvent>(server, client);
			break;
		case Event::NetDisconnect:
			event = std::make_shared<NetServerDisconnectEvent>(server, client);
			break;
		case Event::NetGhostUpdate:
			event = std::make_shared<NetServerGhostUpdateEvent>(server, client, nullptr);
			break;
		default:
			return nullptr;
	}

	event->read(data);
	return event;
}

bool NetServerEvent::write(CharStream &data) const {
	if (!NetEvent::write(data)) {
		return false;
	}

	//So we can identify as a NetServerEvent
	data.push<U8>(Magic);
	return true;
}

bool NetServerEvent::read(CharStream &data) {
	if (!NetEvent::read(data)) {
		return false;
	}

	//Make sure it's a NetServerEvent
	if (data.pop<U8>() != Magic)
		return false;

	//Nothing special here
	return true;
}

NetServerConnectEvent::NetServerConnectEvent(Server *server, ClientConnection *client) : NetServerEvent(NetConnect, server, client) {
}

NetServerDisconnectEvent::NetServerDisconnectEvent(Server *server, ClientConnection *client) : NetServerEvent(NetDisconnect, server, client) {
}

NetServerGhostCreateEvent::NetServerGhostCreateEvent(Server *server, ClientConnection *client, NetObject *object) : NetServerEvent(NetGhostCreate, server, client), mObject(object) {
}

bool NetServerGhostCreateEvent::write(CharStream &data) const {
	if (!NetServerEvent::write(data)) {
		return false;
	}

	data.push<U32>(mServer->getGhostIndex(mObject));
	data.push<std::string>(mObject->getClassRep()->getName());

	if (!mObject->write(data)) {
		return false;
	}

	//Read the packet from the server.
	if (!mObject->writeServerPacket(data))
		return false;

	return true;
}

bool NetServerGhostCreateEvent::read(CharStream &data) {
	//Clients can't create ghosted objects
	return false;
}

NetServerGhostEvent::NetServerGhostEvent(Type type, Server *server, ClientConnection *client, NetObject *object) : NetServerEvent(type, server, client), mObject(object) {
}

bool NetServerGhostEvent::write(CharStream &data) const {
	if (!NetServerEvent::write(data)) {
		return false;
	}

	//Can't exactly have a ghost event without an object
	if (!mObject)
		return false;

	//Push the ghost's id to the stream
	U32 ghostId = mServer->getGhostIndex(mObject);
	data.push<U32>(ghostId);

	return true;
}

bool NetServerGhostEvent::read(CharStream &data) {
	if (!NetServerEvent::read(data)) {
		return false;
	}

	//Find our object in the server
	U32 ghostId = data.pop<U32>();
	mObject = mServer->getGhostedObject(ghostId);

	//Doesn't exist?
	if (mObject == nullptr)
		return false;

	return true;
}

NetServerGhostUpdateEvent::NetServerGhostUpdateEvent(Server *server, ClientConnection *client, NetObject *obj) : NetServerGhostEvent(NetGhostUpdate, server, client, obj) {
}

bool NetServerGhostUpdateEvent::write(CharStream &stream) const {
	if (!NetServerGhostEvent::write(stream))
		return false;

	//Write the object's data so that it can be sent to the server.
	if (!mObject->writeServerPacket(stream))
		return false;

	return true;
}

bool NetServerGhostUpdateEvent::read(CharStream &stream) {
	if (!NetServerGhostEvent::read(stream))
		return false;

	//Read the packet from the server.
	if (!mObject->readClientPacket(stream))
		return false;

	return true;
}

NetServerGhostControlObjectEvent::NetServerGhostControlObjectEvent(Server *server, ClientConnection *client, NetObject *obj) : NetServerGhostEvent(NetGhostControlObject, server, client, obj) {
}

bool NetServerGhostControlObjectEvent::write(CharStream &stream) const {
	if (!NetServerGhostEvent::write(stream))
		return false;
	
	return true;
}

bool NetServerGhostControlObjectEvent::read(CharStream &stream) {
	if (!NetServerGhostEvent::read(stream))
		return false;

	//Clients don't set their control object, we do
	return false;
}