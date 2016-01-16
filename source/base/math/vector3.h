//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATH_VECTOR3_H_
#define _BASE_MATH_VECTOR3_H_

#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>

// SSE support
#ifdef MATH_USE_SIMD
#include <glm/gtx/simd_vec4.hpp>
#endif

#include "base/types.h"

class Vec3 : public glm::vec3 {
public:
	Vec3() : glm::vec3() {}
	Vec3(const glm::vec3 &vec) : glm::vec3(vec) {}
	Vec3(const F32 &val) : glm::vec3(val, val, val) {}
	Vec3(const F32 &x, const F32 &y, const F32 &z) : glm::vec3(x, y, z) {}

	Vec3& operator+(const Vec3 &vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vec3& operator-(const Vec3 &vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vec3& operator*(const F32 &scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vec3& operator/(const F32 &scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	void operator+=(const Vec3 &vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator-=(const Vec3 &vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	void operator*=(const F32 &scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	void operator/=(const F32 &scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}

	std::string toString() const;

	static Vec3 cross(const Vec3 &a, const Vec3 &b);

	static Vec3 project(const Vec3 &a, const Vec3 &b);

	static Vec3 normalize(const Vec3 &vec);

	static F32 angle(const Vec3 &a, const Vec3 &b);

	static F32 distance(const Vec3 &a, const Vec3 &b);

	static F32 dot(const Vec3 &a, const Vec3 &b);

	F32 length();

	/**
	 * Optional SIMD support.
	 * If we do not want to use it or our platform/compiler doesn't support it,
	 * we simply fall back to using scalar representation.
	*/
#ifdef MATH_USE_SIMD
	/**
	 * Force inlining the functions here, as these deal with SIMD data.
	 * we want SIMD to be as fast as possible.
	 */
	#ifdef _MSC_VER
		#define MATH_SIMD_FORCE_INLINE __forceinline
	#elif defined(__llvm__) || defined(__GNUC__)
		// http://stackoverflow.com/questions/5223690/cs-inline-how-strong-a-hint-is-it-for-gcc-and-clang-llvm
		#define MATH_SIMD_FORCE_INLINE __attribute__((always_inline))
	#else
		#warning "MATH_SIMD_FORCE_INLINE is not forcing inline. The regular inline specifier is used."
		#define MATH_SIMD_FORCE_INLINE inline
	#endif
private:
	/**
	 * Create a Vec3 from a SIMD representation.
	 */
	Vec3(const glm::simdVec4 &vec) : glm::vec3(vec.Data.m128_f32[0], vec.Data.m128_f32[1], vec.Data.m128_f32[2]) {}

	/**
	 * Perform a SIMD<->Scalar cast.
	 */
	template<typename A, typename B>
	MATH_SIMD_FORCE_INLINE A simd_cast(const B &vec);

	/**
	 * Casts a Vec3 to a SIMD vector.
	 * @param vec The scalar vector to convert.
	 * @return the SIMD vector.
	 */
	template<> MATH_SIMD_FORCE_INLINE glm::simdVec4 simd_cast(const Vec3 &vec) {
		return glm::simdVec4(vec.x, vec.y, vec.z, 1.0f);
	}

	/**
	 * Casts a SIMD vector to a Vec3.
	 * @param vec The SIMD vector to convert.
	 * @return the Scalar vector.
	 */
	template<> MATH_SIMD_FORCE_INLINE Vec3 simd_cast(const glm::simdVec4 &vec) {
		return Vec3(vec.Data.m128_f32[0], vec.Data.m128_f32[1], vec.Data.m128_f32[2]);
	}
#endif
};

#endif // _BASE_MATH_VECTOR3_H_