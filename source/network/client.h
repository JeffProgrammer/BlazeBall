//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_CLIENT_H_
#define _NETWORK_CLIENT_H_

#include "base/types.h"
#include <enetpp/client.h>

class Client {
public:
	Client(const std::string &serverAddress, const U16 port);
	~Client();

	void connect();
	void disconnect();

	void pollEvents(bool hasPressedSpaceBar);

protected:
	enetpp::client mClient;
	std::string mServerAddress;
	U16 mPort;
};

#endif
