//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef SDLTimer_h
#define SDLTimer_h

#include <stdio.h>
#include "platform/timer.h"
#include "platformSDL/SDLConfig.h"

class SDLTimer : public Timer {
private:
	/// Internal timer tracked.
	U64 mStartTime;
	
	U64 mEndTime;
public:
	SDLTimer() : mStartTime(0), mEndTime(0) {}
	virtual ~SDLTimer() {}
	
	/// Starts the timer and keeps track of the time.
	/// Do not call getDelta() until end() is called.
	inline virtual void start() {
		mStartTime = SDL_GetPerformanceCounter();
	}
	
	/// Ends the timer.
	inline virtual void end() {
		mEndTime = SDL_GetPerformanceCounter() - mStartTime;
	}
	
	/// Gets the time delta between the start() and end() calls.
	/// @return the delta between the timer calls.
	inline virtual F64 getDelta() const {
		return static_cast<F64>((mEndTime * 1000) / static_cast<F64>(SDL_GetPerformanceFrequency()));
	}
};

#endif
