//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// Copyright (c) 2015 Whirligig231
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

#include "physics/bullet/btPhysicsEngine.h"
#include "physics/bullet/btPhysicsInterior.h"
#include "game/gameInterior.h"
#include <vector>

extern std::vector<ShapeInfo> shapes;
extern std::vector<BodyInfo> bodies;
extern std::vector<BodyMovement> moves;

/// BEHOLD, THE MAGIC NUMBER
#define ADJACENCY_NORMAL_THRESHOLD 0.01f

btPhysicsInterior::btPhysicsInterior(GameInterior *interior) : btPhysicsBody(), mInterior(interior) {
    construct();
}

void btPhysicsInterior::construct() {
    //Create body
    btMotionState *state = new btDefaultMotionState();
    btTriangleMesh *mesh = new btTriangleMesh;
	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;

	const DIF::Interior &interior = mInterior->getInterior();
	const U32 size = interior.surface.size();
    for (U32 i = 0; i < size; i++) {
		const DIF::Interior::Surface &surface = interior.surface[i];
		const U32 count = surface.windingCount - 2;
		U32 windingStart = surface.windingStart;
		
        for (U32 j = 0; j < count; j++) {
			if (j % 2 == 0) {
				point0 = interior.point[interior.index[j + windingStart + 2]];
				point1 = interior.point[interior.index[j + windingStart + 1]];
				point2 = interior.point[interior.index[j + windingStart + 0]];
			} else {
				point0 = interior.point[interior.index[j + windingStart + 0]];
				point1 = interior.point[interior.index[j + windingStart + 1]];
				point2 = interior.point[interior.index[j + windingStart + 2]];
			}

            mesh->addTriangle(btConvert(point0), btConvert(point1), btConvert(point2), true);
        }
    }

    btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(mesh, true, true);
    shape->setMargin(0.01f);
    
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, 0));
    
    state->setWorldTransform(transform);
    
    mActor = new btRigidBody(0, state, shape);
    mActor->setRestitution(0.7f);
    mActor->setFriction(1.1f);
    mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    
    ShapeInfo info;
    info.shape = shape;
    shapes.push_back(info);
    
    BodyInfo infoo;
    infoo.body = mActor;
    infoo.collisionNormal = btVector3(0.0f, 0.0f, 0.0f);
    infoo.isDynamic = false;
    infoo.shape = info;
    bodies.push_back(infoo);
}