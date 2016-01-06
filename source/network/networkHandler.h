//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_NETWORKHANDLER_H_
#define _NETWORK_NETWORKHANDLER_H_

#include "base/types.h"
#include <enetpp/server.h>

class NetworkHandler {
public:
	NetworkHandler();
	~NetworkHandler();

	void start();
	void stop();

	void pollEvents();

protected:
	static U32 sUniqueId;

	struct Connection {
		U32 id;
		std::string ipAddress;

		// This is needed for enet internally.
		U32 get_id() { return id; }
	};

	enetpp::server<Connection> mServer;
};

#endif
