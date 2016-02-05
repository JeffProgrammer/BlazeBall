//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_EVENT_NETCLIENTEVENT_H_
#define _NETWORK_EVENT_NETCLIENTEVENT_H_

#include "network/event/netEvent.h"
#include "network/netObject.h"

class Client;

class NetClientEvent : public NetEvent {
public:
	Client *mClient;

	NetClientEvent(Type type, Client *client) : NetEvent(type), mClient(client) {}

	static std::shared_ptr<NetClientEvent> deserialize(CharStream &data, Client *client);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetClientConnectEvent : public NetClientEvent {
public:
	NetClientConnectEvent(Client *client);
};

class NetClientDisconnectEvent : public NetClientEvent {
public:
	NetClientDisconnectEvent(Client *client);
};

class NetClientGhostCreateEvent : public NetClientEvent {
	NetObject *mObject;
public:
	NetClientGhostCreateEvent(Client *client);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetClientGhostEvent : public NetClientEvent {
protected:
	NetObject *mObject;
public:
	NetClientGhostEvent(Type type, Client *client, NetObject *object);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetClientGhostUpdateEvent : public NetClientGhostEvent {
public:
	NetClientGhostUpdateEvent(Client *client, NetObject *obj);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};

class NetClientGhostControlObjectEvent : public NetClientGhostEvent {
public:
	NetClientGhostControlObjectEvent(Client *client, NetObject *obj);

	virtual bool write(CharStream &data) const override;
	virtual bool read(CharStream &data) override;
};


#endif
