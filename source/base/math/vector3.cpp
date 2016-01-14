//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "base/math/vector3.h"

std::string Vec3::toString() const {
	return "[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
}

Vec3 Vec3::cross(const Vec3 &vec) {
#ifdef MATH_USE_SIMD
	return simd_cast<Vec3>(glm::cross(simd_cast<glm::simdVec4>(*this), simd_cast<glm::simdVec4>(vec)));
#else
	return glm::cross(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(vec));
#endif
}

Vec3 Vec3::project(const Vec3 &vec) {
	// Currently GLM doesn't have any SIMD function for glm::proj
	return glm::proj(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(vec));
}

F32 Vec3::distance(const Vec3 &vec) {
#ifdef MATH_USE_SIMD
	return glm::distance(simd_cast<glm::simdVec4>(*this), simd_cast<glm::simdVec4>(vec));
#else
	return glm::distance(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(vec));
#endif
}

F32 Vec3::dot(const Vec3 &vec) {
#ifdef MATH_USE_SIMD
	return glm::simdDot(simd_cast<glm::simdVec4>(*this), simd_cast<glm::simdVec4(vec));
#else
	return glm::dot(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(vec));
#endif
}

F32 Vec3::length() {
#ifdef MATH_USE_SIMD
	return glm::length(simd_cast<glm::simdVec4>(*this));
#else
	return glm::length(static_cast<glm::vec3>(*this));
#endif
}