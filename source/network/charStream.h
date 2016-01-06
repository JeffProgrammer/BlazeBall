//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_CHARSTREAM_H_
#define _NETWORK_CHARSTREAM_H_

#include <assert.h>
#include <vector>
#include <deque>

class CharStream {
	std::deque<U8> mData;
public:
	CharStream() {

	}
	explicit CharStream(const std::vector<U8> &data) {
		mData.insert(mData.end(), data.begin(), data.end());
	}
	CharStream(const U8 *data, const U32 &length) {
		for (U32 i = 0; i < length; i ++) {
			mData.push_back(data[i]);
		}
	}

	template <typename T>
	T push(const T &value);

	template <typename T>
	T pop();

	template <typename T>
	T peek();

	std::vector<U8> getBuffer() const {
		std::vector<U8> data;
		data.insert(data.end(), mData.begin(), mData.end());
		return data;
	}
};


template<>
inline U8 CharStream::push(const U8 &value) {
	mData.push_back(value);
	return value;
}

template<>
inline U8 CharStream::pop() {
	if (mData.size() == 0) {
		assert(false);
		return 0;
	}

	U8 value = mData.front();
	mData.pop_front();
	return value;
}

template<>
inline U8 CharStream::peek() {
	if (mData.size() == 0) {
		assert(false);
		return 0;
	}

	return mData.front();
}

template<>
inline U32 CharStream::push(const U32 &value) {
	union {
		U32 u32;
		U8 u8[4];
	} data;
	data.u32 = value;

	push<U8>(data.u8[0]);
	push<U8>(data.u8[1]);
	push<U8>(data.u8[2]);
	push<U8>(data.u8[3]);
	return value;
}

template<>
inline U32 CharStream::pop() {
	union {
		U32 u32;
		U8 u8[4];
	} data;
	data.u8[0] = pop<U8>();
	data.u8[1] = pop<U8>();
	data.u8[2] = pop<U8>();
	data.u8[3] = pop<U8>();
	return data.u32;
}

#endif
