//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_MOVEMENT_H_
#define _GAME_MOVEMENT_H_

#include "base/types.h"

struct Movement {
	Movement() : forward(false), backward(false), left(false), right(false), jump(false), pitch(0.0f), yaw(0.0f), pitchUp(false), pitchDown(false), yawLeft(false), yawRight(false), fire(false) {

	}

	bool forward;
	bool backward;
	bool left;
	bool right;
	bool jump;

	F32 pitch;
	F32 yaw;

	bool pitchUp;
	bool pitchDown;
	bool yawLeft;
	bool yawRight;

	bool fire;
};

#endif // _GAME_MOVEMENT_H_
