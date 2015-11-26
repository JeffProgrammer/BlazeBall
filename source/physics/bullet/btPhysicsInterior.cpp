//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// Copyright (c) 2015 Whirligig231
// All rights reserved.
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
	btCompoundShape *shape = new btCompoundShape;

	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;

	const DIF::Interior &interior = mInterior->getInterior();
	const U32 size = interior.convexHull.size();

	for (U32 i = 0; i < size; i ++) {
		const DIF::Interior::ConvexHull &hull = interior.convexHull[i];

		btConvexHullShape *hullShape = new btConvexHullShape;

		const U32 count = hull.hullCount;
		for (U32 j = 0; j < count; j ++) {
			DIF::Point3F point = interior.point[interior.hullIndex[hull.hullStart + j]];
			hullShape->addPoint(btConvert(point));
		}

		shape->addChildShape(btTransform::getIdentity(), hullShape);
	}

    shape->setMargin(0.01f);
    
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, 0));
    
    state->setWorldTransform(transform);
    
    mActor = new btRigidBody(0, state, shape);
    mActor->setRestitution(1.f);
    mActor->setFriction(1.f);
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