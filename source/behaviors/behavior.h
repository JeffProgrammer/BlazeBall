//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BEHAVIORS_BEHAVIOR_H_
#define _BEHAVIORS_BEHAVIOR_H_

class Behavior {
public:
public:
	virtual void onStart() = 0;
	virtual void onUpdate(const F64 &dt) = 0;
};

#endif // _BEHAVIORS_BEHAVIOR_H_