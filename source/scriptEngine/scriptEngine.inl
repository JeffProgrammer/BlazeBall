//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _WIN32
#define sprintf_s snprintf
#endif

//-----------------------------------------------------------------------------
// Setters and getters
//-----------------------------------------------------------------------------

#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4800) /* forcing value to bool 'true' or 'false' (performance warning) */
#pragma warning(disable : 4244) /* conversion from 'double' to 'F32', possible loss of data */
#endif

template<typename T> std::string scriptGetter(void *ptr);
template<typename T> bool scriptSetter(void *ptr, const std::string &value);

#define EasyGetterSetter(type, getter, setter) \
template<> inline std::string scriptGetter<type>(void *ptr) { \
    return getter(*static_cast<type *>(ptr)); \
} \
template<> inline bool scriptSetter<type>(void *ptr, const std::string &value) { \
    *static_cast<type *>(ptr) = setter(value.c_str()); \
	return true; \
}

EasyGetterSetter(bool, std::to_string, atoi);
EasyGetterSetter(S8,   std::to_string, atoi);
EasyGetterSetter(U8,   std::to_string, atoi);
EasyGetterSetter(S16,  std::to_string, atoi);
EasyGetterSetter(U16,  std::to_string, atoi);
EasyGetterSetter(S32,  std::to_string, atoi);
EasyGetterSetter(U32,  std::to_string, atoi);
EasyGetterSetter(S64,  std::to_string, atoll);
EasyGetterSetter(U64,  std::to_string, atoll);
EasyGetterSetter(F32,  std::to_string, atof);
EasyGetterSetter(F64,  std::to_string, atof);

EasyGetterSetter(std::string,,); //Strings work by default

#undef EasyGetterSetter

/**
 * Vectors
 */
template<> inline std::string scriptGetter<glm::vec3>(void *ptr) {
	const glm::vec3 &vec = *static_cast<glm::vec3 *>(ptr);
	char buff[64];
	sprintf_s(buff, 64, "%g %g %g", vec.x, vec.y, vec.z);
	return std::string(buff);
}
template<> inline bool scriptSetter<glm::vec3>(void *ptr, const std::string &value) {
	glm::vec3 *vector = static_cast<glm::vec3 *>(ptr);
	sscanf(value.c_str(), "%g %g %g", &vector->x, &vector->y, &vector->z);
	return true; 
}

/**
 * Quaternions
 */
template<> inline std::string scriptGetter<glm::quat>(void *ptr) {
	const glm::quat &quat = *static_cast<glm::quat *>(ptr);
	char buff[64];
	sprintf_s(buff, 64, "%g %g %g %g", quat.x, quat.y, quat.z, quat.w);
	return std::string(buff);
}
template<> inline bool scriptSetter<glm::quat>(void *ptr, const std::string &value) {
	glm::quat *quat = static_cast<glm::quat *>(ptr);
	sscanf(value.c_str(), "%g %g %g %g", &quat->x, &quat->y, &quat->z, &quat->w);
	return true;
}

#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning(pop)
#endif