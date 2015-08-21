//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
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
