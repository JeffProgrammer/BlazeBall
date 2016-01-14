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

int main(int argc, const char **argv) {
	// Initialize the abstract class rep system
	AbstractClassRep::init();

	GameState state(std::make_unique<SDLPlatform>(std::make_unique<Platform::ConcretePhysicsEngineFactory<btPhysicsEngine>>()));

	// Load the networking engine
	Network::init();

	std::vector<std::pair<U32, U32>>test;
	BitStream stream;

	for (U32 i = 0; i < 100; i ++) {
		U32 bits = arc4random_uniform(31) + 1;
		U32 value = arc4random();

		stream.push(value, bits);
		value = value & (0xFFFFFFFF >> (32 - bits));
		test.push_back(std::make_pair(value, bits));
		IO::printf("Insert: %d / %d bits\n", value, bits);
	}

	stream.reset();

	for (U32 i = 0; i < 100; i ++) {
		auto pair = test[i];
		assert(stream.peek(pair.second) == pair.first);
		assert(stream.pop(pair.second) == pair.first);
		IO::printf("Success: %d / %d bits\n", pair.first, pair.second);
	}

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