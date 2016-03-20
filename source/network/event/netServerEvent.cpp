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

#include "netServerEvent.h"
#include "network/server.h"
#include "base/io.h"

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