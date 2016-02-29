//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "physics/physicsEngine.h"
#include "physics/physicsInterior.h"
#include "game/gameInterior.h"
#include <vector>

PhysicsInterior::PhysicsInterior(GameInterior *interior) : PhysicsBody(), mInterior(interior) {
	construct();
}

void PhysicsInterior::construct() {
    //Create body
	btMotionState *state = new btDefaultMotionState();
	btTriangleMesh *mesh = new btTriangleMesh;
	
	const DIF::Interior &interior = mInterior->getInterior();
	const U32 size = interior.surface.size();

	U32 triIndex = 0;
	for (U32 i = 0; i < size; i++) {
		const DIF::Interior::Surface &surface = interior.surface[i];
		const U32 count = surface.windingCount - 2;
		U32 windingStart = surface.windingStart;
		
		for (U32 j = 0; j < count; j++) {
			U32 v0, v1, v2;
			if (j % 2 == 0) {
				v0 = interior.index[j + windingStart + 2];
				v1 = interior.index[j + windingStart + 1];
				v2 = interior.index[j + windingStart + 0];
			} else {
				v0 = interior.index[j + windingStart + 0];
				v1 = interior.index[j + windingStart + 1];
				v2 = interior.index[j + windingStart + 2];
			}

			const DIF::Point3F &point0 = interior.point[v0];
			const DIF::Point3F &point1 = interior.point[v1];
			const DIF::Point3F &point2 = interior.point[v2];

			mesh->addTriangle(btConvert(point0), btConvert(point1), btConvert(point2), true);

			mTriangleLookup[triIndex].surfaceIndex = i;
			mTriangleLookup[triIndex].vertex[0] = v0;
			mTriangleLookup[triIndex].vertex[1] = v1;
			mTriangleLookup[triIndex].vertex[2] = v2;
			triIndex ++;
		}
	}

	btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(mesh, true, true);
	btTriangleInfoMap *map = new btTriangleInfoMap();
	btGenerateInternalEdgeInfo(shape, map);

	shape->setMargin(0.01f);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, 0, 0));

	state->setWorldTransform(transform);

	mActor = new btRigidBody(0, state, shape);
	mActor->setRestitution(1.f);
	mActor->setFriction(1.f);
	mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}