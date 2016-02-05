//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTMATH_H_
#define _SCRIPTENGINE_SCRIPTMATH_H_

inline glm::vec3 vec3_add_vec3(const glm::vec3 &lhs, const glm::vec3 &rhs) {
	return lhs + rhs;
}
inline glm::vec3 vec3_sub_vec3(const glm::vec3 &lhs, const glm::vec3 &rhs) {
	return lhs - rhs;
}
inline glm::vec3 vec3_mul_float(const glm::vec3 &lhs, const F32 &rhs) {
	return lhs * rhs;
}
inline glm::vec3 vec3_div_float(const glm::vec3 &lhs, const F32 &rhs) {
	return lhs / rhs;
}

inline glm::vec3 vec3_add_vec3_mutate(glm::vec3 &lhs, const glm::vec3 &rhs) {
	return lhs += rhs;
}
inline glm::vec3 vec3_sub_vec3_mutate(glm::vec3 &lhs, const glm::vec3 &rhs) {
	return lhs -= rhs;
}
inline glm::vec3 vec3_mul_float_mutate(glm::vec3 &lhs, const F32 &rhs) {
	return lhs *= rhs;
}
inline glm::vec3 vec3_div_float_mutate(glm::vec3 &lhs, const F32 &rhs) {
	return lhs /= rhs;
}

std::string vec3_to_string(const glm::vec3 &vec) {
	return glm::to_string(vec);
}


#endif
