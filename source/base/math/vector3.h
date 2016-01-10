//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATH_VECTOR3_H_
#define _BASE_MATH_VECTOR3_H_

#include <glm/glm.hpp>
#include "base/types.h"

class Vector3 : public glm::vec3 {
public:
	Vector3() : glm::vec3() {}
	Vector3(const glm::vec3 &vec) : glm::vec3(vec) {}
	Vector3(const F32 &x, const F32 &y, const F32 &z) : glm::vec3(x, y, z) {}

	Vector3& operator+(const Vector3 &vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3& operator-(const Vector3 &vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& operator*(const F32 &scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3& operator/(const F32 &scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	void operator+=(const Vector3 &vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator-=(const Vector3 &vec) {
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