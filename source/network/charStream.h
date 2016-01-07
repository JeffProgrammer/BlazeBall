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

//-----------------------------------------------------------------------------
// U8 datatype. Represents the base type for pushing and popping data off of
// the stream
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// U32 datatype stream
//-----------------------------------------------------------------------------

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

template<>
inline U32 CharStream::peek() {
	union {
		U32 u32;
		U8 u8[4];
	} data;
	data.u8[0] = peek<U8>();
	data.u8[1] = peek<U8>();
	data.u8[2] = peek<U8>();
	data.u8[3] = peek<U8>();
	return data.u32;
}

//-----------------------------------------------------------------------------
// F32 datatype stream
//-----------------------------------------------------------------------------

template<>
inline F32 CharStream::push(const F32 &value) {
	union {
		F32 f32;
		U32 u32;
	} data;
	data.f32 = value;
	
	push<U32>(data.u32);
	return value;
}

template<>
inline F32 CharStream::pop() {
	union {
		F32 f32;
		U32 u32;
	} data;
	data.u32 = pop<U32>();
	return data.f32;
}

template<>
inline F32 CharStream::peek() {
	union {
		F32 f32;
		U32 u32;
	} data;
	data.u32 = peek<U32>();
	return data.f32;
}

//-----------------------------------------------------------------------------
// glm::vec3 datatype stream
//-----------------------------------------------------------------------------

template<>
inline glm::vec3 CharStream::push(const glm::vec3 &value) {
	push<F32>(value.x);
	push<F32>(value.y);
	push<F32>(value.z);
	return value;
}

template<>
inline glm::vec3 CharStream::pop() {
	F32 data[3];
	data[0] = pop<F32>();
	data[1] = pop<F32>();
	data[2] = pop<F32>();
	return glm::vec3(data[0], data[1], data[2]);
}

//-----------------------------------------------------------------------------
// glm::quat datatype stream
//-----------------------------------------------------------------------------

template<>
inline glm::quat CharStream::push(const glm::quat &value) {
	push<F32>(value.x);
	push<F32>(value.y);
	push<F32>(value.z);
	push<F32>(value.w);
	return value;
}

template<>
inline glm::quat CharStream::pop() {
	F32 data[4];
	data[0] = pop<F32>();
	data[1] = pop<F32>();
	data[2] = pop<F32>();
	data[3] = pop<F32>();
	return glm::quat(data[0], data[1], data[2], data[3]);
}

#endif
