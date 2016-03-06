//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_PLATFORMTIMER_H_
#define _PLATFORM_PLATFORMTIMER_H_

#include "base/types.h"
#include <mutex>

/**
 * A timer class that lets one calculate the delta time in seconds between the
 * start and end calls.
 * Interally, the timer class must be implemented as thread-safe. I recommend
 * not sharing Timer objects between threads, though you *probably* can.
 */
class PlatformTimer {
protected:
	/* 
	 * Locks the global timer from being accessed concurrently
	 */
	static std::mutex sMutex;

public:
	virtual ~PlatformTimer() {};
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
	 * Gets the time delta in seconds between the start() and end() calls.
	 * @return the delta in seconds between the timer calls.
	 */
	virtual F64 getDelta() const = 0;

	/**
	 * Gets the elapsed time in seconds since the start method was called.
	 * @return the elapsed time in seconds since it was last called.
	 */
	virtual F64 getElapsedTimeSinceStart() = 0;
};

#endif // _PLATFORM_PLATFORMTIMER_H_