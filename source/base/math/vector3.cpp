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
	return glm::cross(*this, vec);
#endif
}

Vec3 Vec3::project(const Vec3 &vec) {
	return glm::proj(*this, vec);
}

F32 Vec3::length() const {
#ifdef MATH_USE_SIMD
	// couldn't use simd cast here.
	// compiler is fucking with me.
	return glm::length(simd_cast<glm::simdVec4>(*this));
#else
	return glm::length(static_cast<const glm::vec3>(*this));
#endif
}