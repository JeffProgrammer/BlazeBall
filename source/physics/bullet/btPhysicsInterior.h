//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef btPhysicsInterior_h
#define btPhysicsInterior_h

#include "btPhysicsBody.h"

class btPhysicsInterior : public btPhysicsBody {
public:
	struct TriangleInfo {
		U32 surfaceIndex;
		U32 vertex[3];
	};
protected:
	GameInterior *mInterior;
	std::map<U32, TriangleInfo> mTriangleLookup;
public:
	GameInterior *getInterior() { return mInterior; }

	const TriangleInfo &getTriangleInfo(const U32 &triangleIndex) { return mTriangleLookup[triangleIndex]; }
	btPhysicsInterior(GameInterior *interior);
	void construct();
};

#endif /* btPhysicsInterior_h */