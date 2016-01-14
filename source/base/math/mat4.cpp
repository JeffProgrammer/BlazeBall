//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "base/math/mat4.h"
#include "base/math/vector3.h"

Mat4 Mat4::rotate(const F32 &angle, const Vec3 &axis) {
	return glm::rotate(*this, angle, axis);
}

Mat4 Mat4::translate(const Vec3 &vec) {
	return glm::translate(*this, vec);
}

Mat4 Mat4::inverse() {
	return glm::inverse(*this);
}

std::string Mat4::toString() const {
	std::string ret;
	for (U32 i = 0; i < 4; i++) {
		ret += std::to_string((*this)[i].x) + " " + std::to_string((*this)[i].y) +
			" " + std::to_string((*this)[i].z) + " " + std::to_string((*this)[i].w) + "\n";
	}
	return ret;
}