//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_EVENT_NETSERVEREVENT_H_
#define _NETWORK_EVENT_NETSERVEREVENT_H_

#include "network/event/netEvent.h"

class Server;

class NetServerEvent : public NetEvent {
public:
	Server *mServer;

	NetServerEvent(Type type, Server *server) : NetEvent(type), mServer(server) {}

	static NetEvent *deserialize(CharStream &data, Server *server);

	virtual bool write(CharStream &data);
	virtual bool read(CharStream &data);
};

class NetServerConnectEvent : public NetServerEvent {
public:
	NetServerConnectEvent(Server *server) : NetServerEvent(NetServerConnect, server) {}
};

class NetServerDisconnectEvent : public NetServerEvent {
public:
	NetServerDisconnectEvent(Server *server) : NetServerEvent(NetServerDisconnect, server) {}
};

#endif
