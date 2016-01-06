//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef SDLTimer_h
#define SDLTimer_h

#include <stdio.h>
#include "platform/platformTimer.h"
#include "platform/SDL/SDLConfig.h"

class SDLTimer : public PlatformTimer {
private:
	/*
	 * The timestamp at which the timer begins counting at again
	 */
	U64 mStartTime;
	
	/*
	 * The timestamp at which the timer begins counting at again
	 */
	U64 mEndTime;

	/*
	 * The platform specific frequency, stored to convert the delta to a 
	 * proper unit.
	 */
	F64 mFrequency;

	/*
	 * Gets the platform specific frequency of the system counter.
	 * This is stored as a property on the timer and is only called by the
	 * constructor.
	 */
	F64 getFrequency() {
		std::lock_guard<std::mutex> guard(sMutex);
		return static_cast<F64>(SDL_GetPerformanceFrequency());
	}
public:
	SDLTimer() {
		mStartTime = 0;
		mEndTime = 0;
		mFrequency = getFrequency();
	}
	
	/*
	 * Starts the timer and keeps track of the time.
	 * Do not call getDelta() until end() is called.
	 */
	virtual void start() {
		std::lock_guard<std::mutex> guard(sMutex);
		mStartTime = SDL_GetPerformanceCounter();
	}
	
	/*
	 * Ends the timer.
	 */
	virtual void end() {
		std::lock_guard<std::mutex> guard(sMutex);
		mEndTime = SDL_GetPerformanceCounter() - mStartTime;
	}
	
	/*
	 * Gets the time delta between the start() and end() calls.
	 * @return the delta between the timer calls.
	 */
	virtual F64 getDelta() const {
		return (static_cast<F64>((mEndTime * 1000) / mFrequency)) / 1000.0;
	}
};

#endif
