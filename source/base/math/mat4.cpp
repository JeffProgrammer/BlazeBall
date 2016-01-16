//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "base/math/mat4.h"
#include "base/math/vector3.h"

Mat4 Mat4::rotate(const Mat4 &mat, const F32 &angle, const Vec3 &axis) {
	return glm::rotate(mat, angle, axis);
}

Mat4 Mat4::translate(const Mat4 &mat, const Vec3 &vec) {
	return glm::translate(mat, vec);
}

Mat4 Mat4::inverse(const Mat4 &mat) {
#ifdef MATH_USE_SIMD
	return glm::inverse(simd_cast<glm::simdMat4>(mat));
#else
	return glm::inverse(mat);
#endif
}

Mat4 Mat4::scale(const Mat4 &mat, const Vec3 &vec) {
	return glm::scale(mat, vec);
}

void Mat4::operator*=(const Mat4 &mat) {
#ifdef MATH_USE_SIMD
	(*this) = glm::matrixCompMult(simd_cast<glm::simdMat4>(*this), simd_cast<glm::simdMat4>(mat));
#else
	(*this) = static_cast<const glm::mat4 &>(*this) * static_cast<const glm::mat4 &>(mat);
#endif
}

std::string Mat4::toString() const {
	std::string ret;
	const Mat4 &mat = (*this);
	for (U32 i = 0; i < 4; i++) {
		ret += std::to_string(mat[i].x) + " " + std::to_string(mat[i].y) +
			" " + std::to_string(mat[i].z) + " " + std::to_string(mat[i].w) + "\n";
	}
	return ret;
}