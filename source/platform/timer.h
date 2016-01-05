//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef timer_h
#define timer_h

#include "base/types.h"

class Timer {
public:
	/* 
	 * Starts the timer and keeps track of the time.
	 * Do not call getDelta() until end() is called.
	 */
	virtual void start() = 0;
	
	/*
	 * Ends the timer.
	 */
	virtual void end() = 0;
	
	/* 
	 * Gets the time delta between the start() and end() calls.
	 * @return the delta between the timer calls.
	 */
	virtual F64 getDelta() const = 0;
};

#endif
