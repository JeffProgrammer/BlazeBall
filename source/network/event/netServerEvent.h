//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
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


#endif
