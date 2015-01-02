//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef point3F_h
#define point3F_h

#include <math.h>

class Point3F {
public:
	F32 x;
	F32 y;
	F32 z;

	Point3F() : x(0), y(0), z(0) {};
	Point3F(F32 x, F32 y, F32 z) : x(x), y(y), z(z) {};

	Point3F  operator+ (const Point3F &point) const;
	Point3F &operator+=(const Point3F &point);
	Point3F  operator- (const Point3F &point) const;
	Point3F &operator-=(const Point3F &point);
	Point3F  operator* (const F32 scalar) const;
	Point3F &operator*=(const F32 scalar);
	Point3F  operator/ (const F32 scalar) const;
	Point3F &operator/=(const F32 scalar);

	F32 dot(const Point3F &point) const;
	Point3F cross(const Point3F &point) const;
	Point3F project(const Point3F &point) const;
	Point3F reject(const Point3F &point) const;

	F32 length() const;
	F32 distance(const Point3F &point) const;
	F32 angle(const Point3F &point) const;

	Point3F convert() const;
	Point3F roundThousands() const;
};

inline Point3F Point3F::operator+(const Point3F &point) const {
	return Point3F(x + point.x, y + point.y, z + point.z);
}

inline Point3F &Point3F::operator+=(const Point3F &point) {
	x += point.x, y += point.y, z += point.z;
	return *this;
}

inline Point3F Point3F::operator-(const Point3F &point) const {
	return Point3F(x - point.x, y - point.y, z - point.z);
}

inline Point3F &Point3F::operator-=(const Point3F &point) {
	x -= point.x, y -= point.y, z -= point.z;
	return *this;
}

inline Point3F Point3F::operator*(const F32 scalar) const {
	return Point3F(x * scalar, y * scalar, z * scalar);
}

inline Point3F &Point3F::operator*=(const F32 scalar) {
	x *= scalar, y *= scalar, z *= scalar;
	return *this;
}

inline Point3F Point3F::operator/(const F32 scalar) const {
	return Point3F(x / scalar, y / scalar, z / scalar);
}

inline Point3F &Point3F::operator/=(const F32 scalar) {
	x /= scalar, y /= scalar, z /= scalar;
	return *this;
}

inline F32 Point3F::dot(const Point3F &point) const {
	return x * point.x + y * point.y + z * point.z;
}

inline Point3F Point3F::cross(const Point3F &point) const {
	return Point3F((y * point.z) - (z * point.y),
	               (z * point.x) - (x * point.z),
	               (x * point.y) - (y * point.x));
}

inline Point3F Point3F::project(const Point3F &point) const {
	return point * (dot(point) / dot(*this));
}

inline Point3F Point3F::reject(const Point3F &point) const {
	return *this - project(point);
}

inline F32 Point3F::length() const {
	return sqrtf(x*x + y*y + z*z);
}

inline F32 Point3F::distance(const Point3F &point) const {
	return (*this - point).length();
}

inline F32 Point3F::angle(const Point3F &point) const {
	return acosf(dot(point) / length() / point.length());
}

inline Point3F Point3F::convert() const {
	return Point3F(x, z, y);
}

inline Point3F Point3F::roundThousands() const {
	return Point3F(floorf(x * 1000.0f) / 1000.f,
				   floorf(y * 1000.0f) / 1000.f,
				   floorf(z * 1000.0f) / 1000.f);
}

#endif
