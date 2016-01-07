//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netClientEvent.h"
#include "network/client.h"
#include "base/io.h"

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

NetClientGhostEvent::NetClientGhostEvent(Client *client, GameObject *object) : NetClientEvent(NetGhost, client), mObject(object) {
}

bool NetClientGhostEvent::write(CharStream &data) const {
	if (!NetClientEvent::write(data)) {
		return false;
	}

	data.push<glm::vec3>(mObject->getPosition());
	data.push<glm::quat>(mObject->getRotation());
	data.push<glm::vec3>(mObject->getScale());

	return true;
}

bool NetClientGhostEvent::read(CharStream &data) {
	if (!NetClientEvent::read(data)) {
		return false;
	}

	mObject->setPosition(data.pop<glm::vec3>());
	mObject->setRotation(data.pop<glm::quat>());
	mObject->setScale   (data.pop<glm::vec3>());

	return true;
}
