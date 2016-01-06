//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netClientEvent.h"

const U8 Magic = 0x42;

NetEvent *NetClientEvent::deserialize(CharStream &data, Client *client) {
	//Don't corrupt the stream if it's not a net event
	if (data.peek<U8>() != NetEvent::Magic)
		return nullptr;
	//Ok, take off magic
	data.pop<U8>();

	Type type = static_cast<Type>(data.pop<U32>());
	NetEvent *event;

	//TODO: Make this fancy
	switch (type) {
		case Event::NetClientConnect:
			event = new NetClientConnectEvent(client);
			break;
		case Event::NetClientDisconnect:
			event = new NetClientDisconnectEvent(client);
			break;
		default:
			return nullptr;
	}

	event->read(data);
	return event;
}

bool NetClientEvent::write(CharStream &data) {
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