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

#ifndef physics_h
#define physics_h

#include <stdio.h>
#include "types.h"
#include <btBulletDynamicsCommon.h>

class Physics {
	btDiscreteDynamicsWorld *world;
public:
	void init();
	void simulate(F32 delta);
	void addRigidBody(btRigidBody *body);

	inline btDiscreteDynamicsWorld *getWorld() { return world; };

	static Physics *getPhysics();
};

template <typename from, typename to>
static to btConvert(from point);

inline static btVector3 btConvert(Point3F point) {
	return btVector3(point.x, point.y, point.z);
}

inline static Point3F btConvert(btVector3 point) {
	return Point3F(point.x(), point.y(), point.z());
}

inline static btQuaternion btConvert(AngAxisF point) {
	return btQuaternion(btConvert(point.axis), point.angle);
}

inline static AngAxisF btConvert(btQuaternion point) {
	return AngAxisF(btConvert(point.getAxis()), point.getAngle());
}

#endif
