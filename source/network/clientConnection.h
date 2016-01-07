//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_CLIENTCONNECTION_H_
#define _NETWORK_CLIENTCONNECTION_H_

#include <string>
#include "base/types.h"

class ClientConnection {
public:
	U32 id;
	std::string ipAddress;

	// This is needed for enet internally.
	U32 get_id() {
		return id;
	}
};

#endif
