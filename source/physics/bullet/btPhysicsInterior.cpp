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
    
    std::vector<BulletTriangle> triData;
    std::vector<std::pair<int, int> > adjacent;
    U32 index = 0;
    
    for (U32 i = 0; i < mInterior->surface.size(); i ++) {
		DIF::Interior::Surface surface = mInterior->surface[i];
        
        for (U32 j = 0; j < surface.windingCount - 2; j ++) {
			Point3F point0;
			Point3F point1;
			Point3F point2;

			if (j % 2 == 0) {
				point0 = mInterior->point[mInterior->index[j + surface.windingStart + 2]];
				point1 = mInterior->point[mInterior->index[j + surface.windingStart + 1]];
				point2 = mInterior->point[mInterior->index[j + surface.windingStart + 0]];
			} else {
				point0 = mInterior->point[mInterior->index[j + surface.windingStart + 0]];
				point1 = mInterior->point[mInterior->index[j + surface.windingStart + 1]];
				point2 = mInterior->point[mInterior->index[j + surface.windingStart + 2]];
			}

            mesh->addTriangle(btConvert(point0), btConvert(point1), btConvert(point2));
            
            BulletTriangle points;
            points.point0 = btConvert(point0);
            points.point1 = btConvert(point1);
            points.point2 = btConvert(point2);
            for (int k = 0; k < triData.size(); k++) {
                // Figure out if they're adjacent!
                const BulletTriangle &others = triData[k];

                // Count number of matched vertices
                int matches = 0;
                if (points.point0 == others.point0 || points.point0 == others.point1 || points.point0 == others.point2)
                    matches++;
                if (points.point1 == others.point0 || points.point1 == others.point1|| points.point1 == others.point2)
                    matches++;
                if (points.point2 == others.point0 || points.point2 == others.point1 || points.point2 == others.point2)
                    matches++;
                
                // Calculate normals
                btVector3 normal1 = (points.point1 - points.point0).cross(points.point2 - points.point0);
                btVector3 normal2 = (others.point1 - others.point0).cross(others.point2 - others.point0);
                normal1.safeNormalize();
                normal2.safeNormalize();
                btVector3 cross = normal1.cross(normal2);
                
                if (cross.length() < sin(ADJACENCY_NORMAL_THRESHOLD) && matches >= 1) {
                    adjacent.push_back(std::make_pair(index, k));
                    adjacent.push_back(std::make_pair(k, index));
                }
            }
            
            triData.push_back(points);
            index ++;
        }
    }
    
    int count = triData.size();
    
    btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(mesh, true, true);
    btTriangleInfoMap *map = new btTriangleInfoMap();
    
    btGenerateInternalEdgeInfo(shape, map);
    
    shape->setMargin(0.01f);
    
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, 0));
    
    state->setWorldTransform(transform);
    
    mActor = new btRigidBody(0, state, shape);
    mActor->setRestitution(1.0f);
    mActor->setFriction(1.0f);
    mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    
    ShapeInfo info;
    info.shape = shape;
    info.triangleData = triData;
    info.adjacent = adjacent;
    shapes.push_back(info);
    
    BodyInfo infoo;
    infoo.body = mActor;
    infoo.collisionNormal = btVector3(0.0f, 0.0f, 0.0f);
    infoo.isDynamic = false;
    infoo.shape = info;
    bodies.push_back(infoo);
}