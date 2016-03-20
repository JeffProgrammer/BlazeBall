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

#ifndef _NETWORK_EVENT_NETSERVEREVENT_H_
#define _NETWORK_EVENT_NETSERVEREVENT_H_

#include "network/event/netEvent.h"
#include "network/netObject.h"

class Server;
class ClientConnection;

class NetServerEvent : public NetEvent {
protected:
	Server *mServer;
	ClientConnection *mClient;
public:

	NetServerEvent(Type type, Server *server, ClientConnection *client) : NetEvent(type), mServer(server), mClient(client) {}

	static std::shared_ptr<NetServerEvent> deserialize(CharStream &data, Server *server, ClientConnection *client);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetServerConnectEvent : public NetServerEvent {
public:
	NetServerConnectEvent(Server *server, ClientConnection *client);
};

class NetServerDisconnectEvent : public NetServerEvent {
public:
	NetServerDisconnectEvent(Server *server, ClientConnection *client);
};

class NetServerGhostCreateEvent : public NetServerEvent {
	NetObject *mObject;
public:
	NetServerGhostCreateEvent(Server *server, ClientConnection *client, NetObject *object);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetServerGhostEvent : public NetServerEvent {
protected:
	NetObject *mObject;
public:
	NetServerGhostEvent(Type type, Server *server, ClientConnection *client, NetObject *object);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetServerGhostUpdateEvent : public NetServerGhostEvent {
public:
	NetServerGhostUpdateEvent(Server *server, ClientConnection *client, NetObject *obj);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetServerGhostControlObjectEvent : public NetServerGhostEvent {
public:
	NetServerGhostControlObjectEvent(Server *server, ClientConnection *client, NetObject *obj);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

#endif // _NETWORK_EVENT_NETSERVEREVENT_H_