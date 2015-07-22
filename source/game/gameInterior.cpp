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

#include "game/gameInterior.h"

void Interior::generateMaterials(String directory) {
	//Allocate all textures for the interior
	if (numMaterials) {
		material = new Material*[numMaterials];
		for (U32 i = 0; i < numMaterials; i ++) {
			printf("Generating materials for texture %s\n", (char *)this->materialName[i]);
			String materialName = io->getName(this->materialName[i]);
			String diffuseFile = Texture::find(materialName, directory);

			//If we can't find it, just chuck the lot and keep going.
			if (!diffuseFile.length) {
				fprintf(stderr, "Error in reading bitmap: %s Bitmap not found.\n", (char *)materialName);
				this->material[i] = NULL;
				continue;
			}
			Material *material = new Material(diffuseFile);

			//Find spec/normal

			String normalName = materialName + ".normal";
			String normalFile = Texture::find(normalName, directory);

			if (normalFile.length) {
				material->setNormalTex(io->loadTexture(normalFile));
			}

			String specularName = materialName + ".alpha";
			String specularFile = Texture::find(specularName, directory);

			if (specularFile.length) {
				material->setSpecularTex(io->loadTexture(specularFile));
			}

			//Assign the texture
			this->material[i] = material;
		}
	}
	this->noise = io->loadTexture("noise.jpg");
	this->noise->setTexNum(GL_TEXTURE3);
}

void Interior::generateMesh() {
	mActor = PhysicsEngine::getEngine()->createInterior(this);
	PhysicsEngine::getEngine()->addBody(mActor);
}

void Interior::exportObj(FILE *file) {
	for (U32 j = 0; j < numPoints; j ++) {
		//Build triangles
		Point3F point = this->point[j];
		fprintf(file, "v %g %g %g\n", -point.x, point.z, point.y);
	}

	for (U32 j = 0; j < numNormals; j ++) {
		//Build triangles
		Point3F point = normal[j];
		fprintf(file, "vn %g %g %g\n", -point.x, point.z, point.y);
	}

	fprintf(file, "\n");

	for (U32 surfaceNum = 0; surfaceNum < numSurfaces; surfaceNum ++) {
		Surface surface = this->surface[surfaceNum];

		Point3F normal = this->normal[plane[surface.planeIndex].normalIndex];
		if (surface.planeFlipped) {
			normal *= -1;
		}

		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
			U32 v0, v1, v2;

			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				v0 = index[j];
				v1 = index[j - 1];
				v2 = index[j - 2];
			} else {
				v0 = index[j - 2];
				v1 = index[j - 1];
				v2 = index[j];
			}
			//Build triangles
			fprintf(file, "f");
			fprintf(file, " %d//%d", v0 + 1, plane[surface.planeIndex].normalIndex + 1);
			fprintf(file, " %d//%d", v1 + 1, plane[surface.planeIndex].normalIndex + 1);
			fprintf(file, " %d//%d", v2 + 1, plane[surface.planeIndex].normalIndex + 1);
			fprintf(file, "\n");
		}
	}
}

U32 Interior::rayCast(RayF ray) {
	//	ray = RayF(ray.origin.convert(), ray.direction.convert());

	U32 closest = -1;
	F32 closestDistance = FLT_MAX;

	//	printf("Ray: {(%f,%f,%f),(%f,%f,%f)}\n",ray.origin.x,ray.origin.y,ray.origin.z,ray.direction.x,ray.direction.y,ray.direction.z);

	for (U32 i = 0; i < numSurfaces; i ++) {
		Surface surface = this->surface[i];

		for (U32 j = 0; j < surface.windingCount - 2; j ++) {
			TriangleF triangle;
			triangle.point0 = point[index[surface.windingStart + j]];
			triangle.point1 = point[index[surface.windingStart + j + 1]];
			triangle.point2 = point[index[surface.windingStart + j + 2]];

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

