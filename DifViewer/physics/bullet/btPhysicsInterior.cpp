//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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

#ifdef BUILD_PHYSICS

#include "btPhysicsInterior.h"
#include "interior.h"

btPhysicsInterior::btPhysicsInterior(Interior *interior) : btPhysicsBody(), mInterior(interior) {
	construct();
}

void btPhysicsInterior::construct() {
    //Create body
    btMotionState *state = new btDefaultMotionState();
    btCompoundShape *shape = new btCompoundShape();
    
    btTransform identity;
    identity.setIdentity();
    identity.setOrigin(btVector3(0, 0, 0));
    
    for (U32 i = 0; i < mInterior->numConvexHulls; i ++) {
        ConvexHull hull = mInterior->convexHull[i];
        
        btConvexHullShape *hullShape = new btConvexHullShape();
        
        for (U32 j = 0; j < hull.hullCount; j ++) {
            Point3F vert = mInterior->point[mInterior->hullIndex[j + hull.hullStart]];
            hullShape->addPoint(btConvert(vert));
        }
        
        hullShape->setMargin(0.01f);
        shape->addChildShape(identity, hullShape);
    }
    
    state->setWorldTransform(identity);
    
    mActor = new btRigidBody(0, state, shape);
    mActor->setRestitution(1.0f);
    mActor->setFriction(1.0f);
    mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

#endif /* BUILD_PHYSICS */