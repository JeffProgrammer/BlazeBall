//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include <cstdlib>

#include "physics/bullet/btPhysicsEngine.h"
#include "scriptEngine/scriptEngine.h"
#include "platform/SDL/SDLPlatform.h"
#include "network/network.h"
#include "main/gameState.h"

#include "network/bitStream.h"

std::unique_ptr<GameState> GameState::gState = nullptr;

int main(int argc, const char **argv) {
	// Initialize the abstract class rep system
	AbstractClassRep::init();

	GameState::gState = std::make_unique<GameState>(std::make_unique<SDLPlatform>(std::make_unique<Platform::ConcretePhysicsEngineFactory<btPhysicsEngine>>()));

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