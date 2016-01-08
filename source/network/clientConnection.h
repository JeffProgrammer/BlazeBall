//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_CLIENTCONNECTION_H_
#define _NETWORK_CLIENTCONNECTION_H_

#include <string>
#include "base/types.h"

class Server;
class ClientConnection {
	friend class Server;
public:
	U32 mId;
	std::string mIpAddress;
	Server *mServer;

	// This is needed for enet internally.
	U32 get_id() {
		return mId;
	}
	const std::string &getAddress() const {
		return mIpAddress;
	}
};

#endif
