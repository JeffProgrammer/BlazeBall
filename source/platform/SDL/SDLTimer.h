//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_SDL_SDLTIMER_H_
#define _PLATFORM_SDL_SDLTIMER_H_

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
	 * @return The Frequency of the timer.
	 */
	F64 getFrequency() {
		std::lock_guard<std::mutex> guard(sMutex);
		return static_cast<F64>(SDL_GetPerformanceFrequency());
	}

	/**
	 * Gets the current platform time.
	 * @return The current time.
	 */
	U64 getTime() {
		std::lock_guard<std::mutex> guard(sMutex);
		return SDL_GetPerformanceCounter();
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

	/**
	 * Gets the elapsed time since the start method was called.
	 * @return the elapsed time in seconds since it was last called.
	 */
	virtual F64 getElapsedTimeSinceStart() {
		return (static_cast<F64>(((getTime() - mStartTime) * 1000) / mFrequency)) / 1000.0;
	}
};

#endif // _PLATFORM_SDL_SDLTIMER_H_