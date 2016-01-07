//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_CLIENT_H_
#define _NETWORK_CLIENT_H_

#include <enetpp/client.h>

#include "base/types.h"
#include "network/event/netClientEvent.h"
#include "network/netObject.h"

class Client {
public:
	Client(const std::string &serverAddress, const U16 port);
	~Client();

	void connect();
	void disconnect();

	void pollEvents();

	void sendEvent(const std::shared_ptr<NetClientEvent> &event);

	NetObject *getGhostedObject(U32 index) {
		return mGhostedObjects[index];
	}
	void addGhostedObject(U32 index, NetObject *object) {
		mGhostedObjects[index] = object;
	}

protected:
	enetpp::client mClient;
	std::string mServerAddress;
	U16 mPort;

	std::map<U32, NetObject *> mGhostedObjects;
};

#endif
