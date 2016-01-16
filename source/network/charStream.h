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

#include "base/math.h"

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
inline Vec3 CharStream::push(const Vec3 &value) {
	push<F32>(value.x);
	push<F32>(value.y);
	push<F32>(value.z);
	return value;
}

template<>
inline Vec3 CharStream::pop() {
	F32 data[3];
	data[0] = pop<F32>();
	data[1] = pop<F32>();
	data[2] = pop<F32>();
	return Vec3(data[0], data[1], data[2]);
}

//-----------------------------------------------------------------------------
// glm::vec4 datatype stream
//-----------------------------------------------------------------------------

template<>
inline glm::vec4 CharStream::push(const glm::vec4 &value) {
	push<F32>(value.x);
	push<F32>(value.y);
	push<F32>(value.z);
	push<F32>(value.w);
	return value;
}

template<>
inline glm::vec4 CharStream::pop() {
	F32 data[4];
	data[0] = pop<F32>();
	data[1] = pop<F32>();
	data[2] = pop<F32>();
	data[3] = pop<F32>();
	return glm::vec4(data[0], data[1], data[2], data[3]);
}

//-----------------------------------------------------------------------------
// glm::quat datatype stream
//-----------------------------------------------------------------------------

template<>
inline Quat CharStream::push(const Quat &value) {
	push<F32>(value.x);
	push<F32>(value.y);
	push<F32>(value.z);
	push<F32>(value.w);
	return value;
}

template<>
inline Quat CharStream::pop() {
	F32 data[4];
	data[0] = pop<F32>();
	data[1] = pop<F32>();
	data[2] = pop<F32>();
	data[3] = pop<F32>();
	return Quat(data[0], data[1], data[2], data[3]);
}

//-----------------------------------------------------------------------------
// glm::mat4 datatype stream
//-----------------------------------------------------------------------------

template<>
inline Mat4 CharStream::push(const Mat4 &value) {
	push<glm::vec4>(value[0]);
	push<glm::vec4>(value[1]);
	push<glm::vec4>(value[2]);
	push<glm::vec4>(value[3]);
	return value;
}

template<>
inline Mat4 CharStream::pop() {
	glm::mat4 data;
	data[0] = pop<glm::vec4>();
	data[1] = pop<glm::vec4>();
	data[2] = pop<glm::vec4>();
	data[3] = pop<glm::vec4>();
	return data;
}

//-----------------------------------------------------------------------------
// std::string datatype stream
//-----------------------------------------------------------------------------

template<>
inline std::string CharStream::push(const std::string &value) {
	size_t len = value.length();

	// first part, write the length
	push<U32>(static_cast<U32>(len));

	// second part, each char
	for (size_t i = 0; i < len; i++) {
		push<U8>(static_cast<U8>(value[i]));
	}
	return value;
}

template<>
inline std::string CharStream::pop() {
	// first part, length
	size_t len = static_cast<size_t>(pop<U32>());

	// second part, each character
	std::string data;
	data.resize(len);
	for (size_t i = 0; i < len; i++) {
		data[i] = static_cast<S8>(pop<U8>());
	}
	return data;
}

#endif
