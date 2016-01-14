//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATH_MAT4_H_
#define _BASE_MATH_MAT4_H_

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

// SSE support
#ifdef MATH_USE_SIMD
#include <glm/gtx/simd_mat4.hpp>
#endif

#include "base/types.h"

class Vec3;

class Mat4 : public glm::mat4 {
public:
	Mat4() : glm::mat4() {}
	Mat4(const F32 &identity) : glm::mat4(1.0f) {}
	Mat4(const glm::mat4 &mat) : glm::mat4(mat) {}

	Mat4 rotate(const F32 &angle, const Vec3 &axis);

	Mat4 translate(const Vec3 &vec);

	Mat4 inverse();

	void operator*=(const Mat4 &mat);

	std::string toString() const;

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
	 * Perform a SIMD<->Scalar cast.
	 */
	template<typename A, typename B>
	MATH_SIMD_FORCE_INLINE A simd_cast(const B &mat);

	/**
	 * Casts a Mat4 to a SIMD matrix.
	 * @param vec The scalar matrix to convert.
	 * @return the SIMD matrix.
	 */
	template<> MATH_SIMD_FORCE_INLINE glm::simdMat4 simd_cast(const Mat4 &mat) {
		return glm::simdMat4(mat);
	}

	/**
	 * Casts a SIMD matrix to a Mat4.
	 * @param vec The SIMD matrix to convert.
	 * @return the Scalar matrix.
	 */
	template<> MATH_SIMD_FORCE_INLINE Mat4 simd_cast(const glm::simdMat4 &mat) {
		return glm::mat4(glm::mat4_cast(mat));
	}

	/**
	 * Create a Mat4 from a SIMD representation.
	 */
	Mat4(const glm::simdMat4 &mat) : glm::mat4(simd_cast<glm::mat4>(mat)) {}
#endif
};

#endif // _BASE_MATH_MAT4_H_