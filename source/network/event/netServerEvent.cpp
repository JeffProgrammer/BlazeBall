//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netServerEvent.h"
#include "network/server.h"
#include "base/io.h"

const U8 Magic = 0x42;

std::shared_ptr<NetServerEvent> NetServerEvent::deserialize(CharStream &data, Server *server) {
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
			event = std::make_shared<NetServerConnectEvent>(server);
			break;
		case Event::NetDisconnect:
			event = std::make_shared<NetServerDisconnectEvent>(server);
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

NetServerConnectEvent::NetServerConnectEvent(Server *server) : NetServerEvent(NetConnect, server) {
}

NetServerDisconnectEvent::NetServerDisconnectEvent(Server *server) : NetServerEvent(NetDisconnect, server) {
}

NetServerGhostCreateEvent::NetServerGhostCreateEvent(Server *server, NetObject *object) : NetServerEvent(NetGhostCreate, server), mObject(object) {
}

bool NetServerGhostCreateEvent::write(CharStream &data) const {
	if (!NetServerEvent::write(data)) {
		return false;
	}

	data.push<U32>(mServer->getGhostIndex(mObject));
	data.push<std::string>(mObject->getClassRep()->getName());

	return mObject->write(data);
}

bool NetServerGhostCreateEvent::read(CharStream &data) {
	//Clients can't create ghosted objects
	return false;
}
