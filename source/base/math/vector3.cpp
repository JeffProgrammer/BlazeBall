//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <glm/gtx/vector_angle.hpp>
#include "base/math/vector3.h"

std::string Vec3::toString() const {
	return "[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
}

Vec3 Vec3::cross(const Vec3 &a, const Vec3 &b) {
#ifdef MATH_USE_SIMD
	return simd_cast<Vec3>(glm::cross(simd_cast<glm::simdVec4>(a), simd_cast<glm::simdVec4>(b)));
#else
	return glm::cross(static_cast<glm::vec3>(a), static_cast<glm::vec3>(b));
#endif
}

Vec3 Vec3::project(const Vec3 &a, const Vec3 &b) {
	// Currently GLM doesn't have any SIMD function for glm::proj
	return glm::proj(static_cast<glm::vec3>(a), static_cast<glm::vec3>(b));
}

F32 Vec3::angle(const Vec3 &a, const Vec3 &b) {
	// Currently GLM doesn't have any SIMD function for glm::angle
	return glm::angle(static_cast<glm::vec3>(a), static_cast<glm::vec3>(b));
}

F32 Vec3::distance(const Vec3 &a, const Vec3 &b) {
#ifdef MATH_USE_SIMD
	return glm::distance(simd_cast<glm::simdVec4>(a), simd_cast<glm::simdVec4>(b));
#else
	return glm::distance(static_cast<glm::vec3>(a), static_cast<glm::vec3>(b));
#endif
}

F32 Vec3::dot(const Vec3 &a, const Vec3 &b) {
#ifdef MATH_USE_SIMD
	return glm::simdDot(simd_cast<glm::simdVec4>(a), simd_cast<glm::simdVec4(b));
#else
	return glm::dot(static_cast<glm::vec3>(a), static_cast<glm::vec3>(b));
#endif
}

F32 Vec3::length() {
#ifdef MATH_USE_SIMD
	return glm::length(simd_cast<glm::simdVec4>(*this));
#else
	return glm::length(static_cast<glm::vec3>(*this));
#endif
}