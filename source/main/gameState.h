//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef _MAIN_GAMESTATE_H_
#define _MAIN_GAMESTATE_H_

#include <memory>

#include "platform/SDL/SDLPlatform.h"
#include "game/world.h"
#include "network/server.h"
#include "network/client.h"

class GameState {
public:
	std::unique_ptr<Platform> platform;

	bool runClient;
	bool runServer;

	std::string serverAddress;

	Client *client;
	Server *server;

	World *clientWorld;
	World *serverWorld;

	std::vector<std::string> mCmdArgs;

	GameState(std::unique_ptr<Platform> &&platform) {
		this->platform = std::move(platform);
		//Local server by default
		runClient = true;
		runServer = true;
		serverAddress = "localhost";
	}
	~GameState() {

	}

	static std::unique_ptr<GameState> gState;

	void parseArgs(int argc, const char **argv);

	bool start();
	void stop();
	void runLoop();
};

#endif // _MAIN_GAMESTATE_H_
