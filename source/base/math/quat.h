//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_MATH_QUAT_H_
#define _BASE_MATH_QUAT_H_

#include <glm/glm.hpp>
#include "base/types.h"

class Quat : public glm::quat {
public:
	Quat() : glm::quat() {}
	Quat(const glm::quat &quat) : glm::quat(quat) {}
	Quat(const F32 &x, const F32 &y, const F32 &z, const F32 &w) : glm::quat(x, y, z, w) {}

	std::string toString() const {
		return "[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + std::to_string(w) + "]";
	}
};

#endif // _BASE_MATH_QUAT_H_