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

	std::string toString() const;
};

#endif // _BASE_MATH_MAT4_H_