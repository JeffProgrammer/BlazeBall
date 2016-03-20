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

#ifndef _NETWORK_BITSTREAM_H_
#define _NETWORK_BITSTREAM_H_

#include <assert.h>
#include <vector>
#include <deque>

class BitStream {
	std::deque<U32> mData;
	U8 mShift;
public:
	BitStream() : mShift(0) {
		mData.push_back(0);
	}

	inline void reset() {
		mShift = 0;
	}

	/**
	 * Push an integer onto the stream with a given bit size
	 * @param value    The integer to push
	 * @param bitCount How many bits long the integer is
	 * @return The integer itself
	 */
	inline U32 push(U32 value, const U8 &bitCount = 32) {
		//Sanity checking
		assert(bitCount < 32);
		assert(bitCount > 0);

		//Sanitize value, don't let it be longer than the number of bits we're promised
		value = (value & (0xFFFFFFFF >> (32 - bitCount)));

		//If this value is going to push us onto the next item we need to do
		// some extra fun math.
		if (mShift + bitCount >= 32) {
			//How many bits over 32 are we going to need?
			U32 extra = (mShift + bitCount) % 32;
			//How many bits do we have left before 32?
			U32 remain = bitCount - extra;

			//Get the part of the value that will be pushed onto the current top,
			// should be `remain` bits long.
			U32 first = value & (0xFFFFFFFF >> (32 - remain));
			//Push it on and make sure we start at the next open bit
			mData.back() |= first << mShift;

			//Get the second part of the value that will become the next top, should
			// be `extra` bits long.
			U32 second = (value >> remain) & (0xFFFFFFFF >> (32 - extra));
			//Start a new top with it
			mData.push_back(second);

			//Shift should become however many bits long that new top is
			mShift = extra;
		} else {
			//We don't have to create a new top, we can just slap this one on the
			// end of the original one. OR the bits on, make sure to push them over
			// so they line up, and cut off anything at the end
			mData.back() |= (value << mShift) & (0xFFFFFFFF >> (32 - bitCount - mShift));

			//Just add to the shift
			mShift += bitCount;
		}
		return value;
	}
	/**
	 * Pop an integer off the stream with a given bit length
	 * @param bitCount How many bits long the integer is
	 * @return The integer on top
	 */
	inline U32 pop(const U8 &bitCount = 32) {
		//Sanity checking
		assert(bitCount < 32);
		assert(bitCount > 0);
		//Keep this so we can return it
		U32 result = 0;
		//If this value is going to push us onto the next item we need to do
		// some extra fun math.
		if (mShift + bitCount >= 32) {
			//How many bits over 32 are we going to need?
			U32 extra = (mShift + bitCount) % 32;
			//How many bits do we have left before 32?
			U32 remain = bitCount - extra;

			//Get the first, lower, part of the number, should be stored at the
			// end of the current top. Shift it over so it's in the correct bit
			U32 first = mData.front() >> mShift;
			//Add it to the result
			result = first;
			//Pop the top off because we've used all its bits
			mData.pop_front();

			//If we hit 32 exactly then this will just be extra wasted time. Optimize
			// it out unless we need it.
			if (extra) {
				//Get the second, upper, part of the number from the new top and
				// shift it over so it lines up
				U32 second = (mData.front() & (0xFFFFFFFF >> (32 - extra))) << remain;
				//Or it with the result so we get the final value
				result = result | second;
			}
			//Shift should become however many bits we read from that new top
			mShift = extra;
		} else {
			//We're not popping anything off so we can just grab the bits from
			// the top and have a nice day.
			result = (mData.front() >> mShift) & (0xFFFFFFFF >> (32 - bitCount));

			//Just add to the shift
			mShift += bitCount;
		}
		return result;
	}
	/**
	 * Peek at the top integer on the stream without actually mutating anything
	 * @param bitCount How many bits long the integer is
	 * @return The integer on top
	 */
	inline U32 peek(const U8 &bitCount = 32) const {
		//Sanity checking
		assert(bitCount < 32);
		assert(bitCount > 0);
		//Keep this so we can return it
		U32 result = 0;
		//If this value is going to push us onto the next item we need to do
		// some extra fun math.
		if (mShift + bitCount >= 32) {
			//How many bits over 32 are we going to need?
			U32 extra = (mShift + bitCount) % 32;
			//How many bits do we have left before 32?
			U32 remain = bitCount - extra;

			//Get the first, lower, part of the number, should be stored at the
			// end of the current top. Shift it over so it's in the correct bit
			U32 first = mData.front() >> mShift;
			//Add it to the result
			result = first;

			//If we hit 32 exactly then this will just be extra wasted time. Optimize
			// it out unless we need it.
			if (extra) {
				//Get the second, upper, part of the number from the next top and
				// shift it over so it lines up
				U32 second = (mData[1] & (0xFFFFFFFF >> (32 - extra))) << remain;
				//Or it with the result so we get the final value
				result = result | second;
			}
		} else {
			//We're not popping anything off so we can just grab the bits from
			// the top and have a nice day.
			result = (mData.front() >> mShift) & (0xFFFFFFFF >> (32 - bitCount));
		}
		return result;
	}

	template <typename T>
	T push(const T &value);

	template <typename T>
	T pop();

	template <typename T>
	T peek() const;
};

#endif // _NETWORK_BITSTREAM_H_