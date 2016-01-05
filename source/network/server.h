//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_SERVER_H_
#define _NETWORK_SERVER_H_

#include "base/types.h"
#include "base/io.h"
#include <thread>

class Server {
public:
	Server();
	~Server();

	void start();

	void stop();

private:
	std::thread mServerThread;
	bool mIsRunning;

	void run();
};

#endif
