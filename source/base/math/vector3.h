//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATH_VECTOR3_H_
#define _BASE_MATH_VECTOR3_H_

#include <glm/glm.hpp>
#include "base/types.h"

class Vec3 : public glm::vec3 {
public:
	Vec3() : glm::vec3() {}
	Vec3(const glm::vec3 &vec) : glm::vec3(vec) {}
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

	std::string toString() const {
		return "[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
	}
};

#endif // _BASE_MATH_VECTOR3_H_