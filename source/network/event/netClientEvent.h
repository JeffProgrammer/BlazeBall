//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_EVENT_NETCLIENTEVENT_H_
#define _NETWORK_EVENT_NETCLIENTEVENT_H_

#include "network/event/netEvent.h"

class Client;

class NetClientEvent : public NetEvent {
public:
	Client *mClient;

	NetClientEvent(Type type, Client *client) : NetEvent(type), mClient(client) {}
};

class NetClientConnectEvent : public NetClientEvent {
public:
	NetClientConnectEvent(Type type, Client *client) : NetClientEvent(NetClientConnect, client) {}
};

class NetClientDisconnectEvent : public NetClientEvent {
public:
	NetClientDisconnectEvent(Type type, Client *client) : NetClientEvent(NetClientDisconnect, client) {}
};

#endif
