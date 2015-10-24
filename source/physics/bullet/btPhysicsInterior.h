//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef btPhysicsInterior_h
#define btPhysicsInterior_h

#include "btPhysicsBody.h"

class btPhysicsInterior : public btPhysicsBody {
	GameInterior *mInterior;
	std::map<U32, U32> mTriangleLookup;
public:
	GameInterior *getInterior() { return mInterior; }

	U32 getSurfaceIndexFromTriangleIndex(const U32 &triangleIndex) { return mTriangleLookup[triangleIndex]; }
	btPhysicsInterior(GameInterior *interior);
	void construct();
};

#endif /* btPhysicsInterior_h */