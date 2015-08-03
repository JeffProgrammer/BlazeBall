//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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

#include "game/gameInterior.h"
#include "base/io.h"
#include "render/bitmap/texture.h"
#include <string>
#include <cfloat>

GameInterior::GameInterior(DIF::Interior interior) : mInterior(interior) {
	renderInfo.generated = false;
}

void GameInterior::generateMaterials(std::string directory) {
	//Allocate all textures for the interior
	if (mInterior.materialName.size()) {
		for (U32 i = 0; i < mInterior.materialName.size(); i ++) {
			printf("Generating materials for texture %s\n", mInterior.materialName[i].c_str());
			std::string materialName = IO::getName(mInterior.materialName[i]);
			std::string diffuseFile = Texture::find(directory + DIR_SEP + materialName);

			//If we can't find it, just chuck the lot and keep going.
			if (!diffuseFile.length()) {
				fprintf(stderr, "Error in reading bitmap: %s Bitmap not found.\n", materialName.c_str());
				mMaterialList.push_back(NULL);
				continue;
			}
			std::string normalName = materialName + ".normal";
			std::string normalFile = Texture::find(directory + DIR_SEP + normalName);
			std::string specularName = materialName + ".alpha";
			std::string specularFile = Texture::find(directory + DIR_SEP + specularName);

			Material *material = new Material(diffuseFile, normalFile, specularFile);

			//Assign the texture
			mMaterialList.push_back(material);
		}
	}
	mNoiseTexture = IO::loadTexture("noise.jpg");
	mNoiseTexture->setTexNum(GL_TEXTURE3);
}

void GameInterior::generateMesh() {
	mActor = PhysicsEngine::getEngine()->createInterior(this);
	PhysicsEngine::getEngine()->addBody(mActor);
}

void GameInterior::exportObj(FILE *file) {
	for (U32 j = 0; j < mInterior.point.size(); j ++) {
		//Build triangles
		glm::vec3 point = mInterior.point[j];
		fprintf(file, "v %g %g %g\n", -point.x, point.z, point.y);
	}

	for (U32 j = 0; j < mInterior.normal.size(); j ++) {
		//Build triangles
		glm::vec3 point = mInterior.normal[j];
		fprintf(file, "vn %g %g %g\n", -point.x, point.z, point.y);
	}

	fprintf(file, "\n");

	for (U32 surfaceNum = 0; surfaceNum < mInterior.surface.size(); surfaceNum ++) {
		const DIF::Interior::Surface &surface = mInterior.surface[surfaceNum];

		glm::vec3 normal = mInterior.normal[mInterior.plane[surface.planeIndex].normalIndex];
		if (surface.planeFlipped) {
			normal *= -1;
		}

		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
			U32 v0, v1, v2;

			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				v0 = mInterior.index[j];
				v1 = mInterior.index[j - 1];
				v2 = mInterior.index[j - 2];
			} else {
				v0 = mInterior.index[j - 2];
				v1 = mInterior.index[j - 1];
				v2 = mInterior.index[j];
			}
			//Build triangles
			fprintf(file, "f");
			fprintf(file, " %d//%d", v0 + 1, mInterior.plane[surface.planeIndex].normalIndex + 1);
			fprintf(file, " %d//%d", v1 + 1, mInterior.plane[surface.planeIndex].normalIndex + 1);
			fprintf(file, " %d//%d", v2 + 1, mInterior.plane[surface.planeIndex].normalIndex + 1);
			fprintf(file, "\n");
		}
	}
}

U32 GameInterior::rayCast(RayF ray) {
	//	ray = RayF(ray.origin.convert(), ray.direction.convert());

	U32 closest = -1;
	F32 closestDistance = FLT_MAX;

	//	printf("Ray: {(%f,%f,%f),(%f,%f,%f)}\n",ray.origin.x,ray.origin.y,ray.origin.z,ray.direction.x,ray.direction.y,ray.direction.z);

	for (U32 i = 0; i < mInterior.surface.size(); i ++) {
		const DIF::Interior::Surface &surface = mInterior.surface[i];

		for (U32 j = 0; j < surface.windingCount - 2; j ++) {
			TriangleF triangle;
			triangle.point0 = mInterior.point[mInterior.index[surface.windingStart + j]];
			triangle.point1 = mInterior.point[mInterior.index[surface.windingStart + j + 1]];
			triangle.point2 = mInterior.point[mInterior.index[surface.windingStart + j + 2]];

			F32 distance = ray.distance(triangle);
			if (distance > 0 && distance < closestDistance) {
				//				printf("Found closest triangle (surface %d, offset %d), distance is %f\n", i, j, distance);
				//				printf("Triangle: {(%f,%f,%f),(%f,%f,%f),(%f,%f,%f)}\n",triangle.point0.x,triangle.point0.y,triangle.point0.z,triangle.point1.x,triangle.point1.y,triangle.point1.z,triangle.point2.x,triangle.point2.y,triangle.point2.z);
				closestDistance = distance;
				closest = i;
			}
		}
	}
	
	return closest;
}

