//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include <cstdlib>

#include "physics/PhysicsEngine.h"
#include "scriptEngine/abstractClassRep.h"
#include "platform/SDL/SDLPlatform.h"
#include "network/network.h"
#include "main/gameState.h"

#include "network/bitStream.h"

std::unique_ptr<GameState> GameState::gState = nullptr;

int main(int argc, const char **argv) {
	GameState::gState = std::make_unique<GameState>(std::make_unique<SDLPlatform>());

	// Copy command line arguments into the gamestate
	for (int i = 0; i < argc; i++)
		GameState::gState->mCmdArgs.push_back(argv[i]);

	// Initialize JSON fields
	AbstractClassRep::init();

	// Load the networking engine
	Network::init();

	// parse command line arguments.
	GameState::gState->parseArgs(argc, argv);

	if (!GameState::gState->start())
		return 1;

	GameState::gState->runLoop();
	GameState::gState->stop();

	// destroy the networking engine
	Network::destroy();

	return 0;
}