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

int main(int argc, const char **argv) {
	// Initialize the abstract class rep system
	AbstractClassRep::init();

	GameState state(std::make_unique<SDLPlatform>(std::make_unique<Platform::ConcretePhysicsEngineFactory<btPhysicsEngine>>()));

	// Load the networking engine
	Network::init();

	// parse command line arguments.
	state.parseArgs(argc, argv);

	if (!state.start())
		return 1;

	state.runLoop();
	state.stop();

	// destroy the networking engine
	Network::destroy();

	return 0;
}