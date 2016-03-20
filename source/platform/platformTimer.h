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
	 * Gets the time delta between the start() and end() calls.
	 * @return the delta between the timer calls.
	 */
	virtual F64 getDelta() const = 0;

	/**
	 * Gets the elapsed time since the start method was called.
	 * @return the elapsed time in seconds since it was last called.
	 */
	virtual F64 getElapsedTimeSinceStart() = 0;
};

#endif // _PLATFORM_PLATFORMTIMER_H_