//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_BULLET_BTPHYSICSINTERIOR_H_
#define _GAME_PHYSICS_BULLET_BTPHYSICSINTERIOR_H_

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

#endif // _GAME_PHYSICS_BULLET_BTPHYSICSINTERIOR_H_