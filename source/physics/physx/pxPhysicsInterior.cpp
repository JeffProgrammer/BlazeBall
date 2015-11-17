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

#include "pxPhysicsInterior.h"
#include "game/gameInterior.h"

PxPhysicsInterior::PxPhysicsInterior(GameInterior *interior) : PxPhysicsBody(), mInterior(interior) {
	construct();
}
void PxPhysicsInterior::construct() {
	//Create body
	physx::PxTriangleMeshGeometry geometry;
	physx::PxTriangleMeshDesc meshDesc;
	geometry.meshFlags = physx::PxMeshGeometryFlag::eDOUBLE_SIDED;

	const DIF::Interior &interior = mInterior->getInterior();

	U32 numTriangles = 0;
	for (DIF::Interior::Surface surface : interior.surface) {
		numTriangles += surface.windingCount - 2;
	}

	glm::vec3 *points = new glm::vec3[interior.point.size()];
	U32 *triangles = new U32[numTriangles * 3];

	for (U32 i = 0; i < interior.point.size(); i ++) {
		points[i] = interior.point[i];
	}

	U32 curPoint = 0;
	for (U32 i = 0; i < interior.surface.size(); i ++) {
		const DIF::Interior::Surface &surface = interior.surface[i];

		for (U32 j = 0; j < surface.windingCount - 2; j ++) {
			U32 v0 = 0;
			U32 v1 = 0;
			U32 v2 = 0;

			if ((j % 2) == 1) {
				v0 = interior.index[j + surface.windingStart + 0];
				v1 = interior.index[j + surface.windingStart + 1];
				v2 = interior.index[j + surface.windingStart + 2];
			} else {
				v0 = interior.index[j + surface.windingStart + 2];
				v1 = interior.index[j + surface.windingStart + 1];
				v2 = interior.index[j + surface.windingStart + 0];
			}

			triangles[curPoint++] = v0;
			triangles[curPoint++] = v1;
			triangles[curPoint++] = v2;
		}
	}

	meshDesc.points.count = interior.point.size();
	meshDesc.points.data = points;
	meshDesc.points.stride = sizeof(glm::vec3);

	meshDesc.triangles.count = numTriangles;
	meshDesc.triangles.data = triangles;
	meshDesc.triangles.stride = 3 * sizeof(U32);

	physx::PxDefaultMemoryOutputStream writeBuffer;
	bool status = static_cast<PxPhysicsEngine *>(PhysicsEngine::getEngine())->getPxCooking()->cookTriangleMesh(meshDesc, writeBuffer);

	physx::PxPhysics *physics = static_cast<PxPhysicsEngine *>(PhysicsEngine::getEngine())->getPxPhysics();

	physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	physx::PxTriangleMesh *mesh = physics->createTriangleMesh(readBuffer);
	geometry.triangleMesh = mesh;

	physx::PxMaterial *material = physics->createMaterial(1.0, 1.0, 0.7);
	material->setFrictionCombineMode(physx::PxCombineMode::eMULTIPLY);
	material->setRestitutionCombineMode(physx::PxCombineMode::eMULTIPLY);

	mActor = physics->createRigidStatic(physx::PxTransform(physx::PxIDENTITY()));
	physx::PxShape *shape = mActor->createShape(geometry, *material);
	shape->setContactOffset(0.01f);
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
}
