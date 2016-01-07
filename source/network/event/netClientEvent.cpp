//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netClientEvent.h"
#include "network/client.h"
#include "base/io.h"
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

NetClientGhostCreateEvent::NetClientGhostCreateEvent(Client *client, NetObject *object) : NetClientEvent(NetGhostCreate, client), mObject(object) {
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

	mObject = dynamic_cast<NetObject *>(AbstractClassRep::createFromName(className));
	if (!mObject) {
		return false;
	}
	mObject->mGhostId = ghostId;

	return mObject->read(data);
}
