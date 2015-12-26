//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/gameInterior.h"
#include "base/io.h"
#include "physics/physicsBody.h"
#include "texture/texture.h"
#include <string>
#include <cfloat>

GameInterior::GameInterior(DIF::Interior interior) : RenderedObject(), mInterior(interior) {
	gfxInit();
}

GameInterior::GameInterior() : RenderedObject() {
	gfxInit();
}

GameInterior::~GameInterior() {
	glDeleteBuffers(1, &mVbo);
}

void GameInterior::gfxInit() {
	mRenderInfo.generated = false;
}

void GameInterior::generateMaterials(std::string directory) {
	mNoiseTexture = IO::loadTexture("noise.jpg");

	//TODO: Per-material shaders
	Shader *shader = new Shader("interiorV.glsl", "interiorF.glsl");
	shader->addUniformLocation("textureSampler", 0);
	shader->addUniformLocation("normalSampler", 1);
	shader->addUniformLocation("specularSampler", 2);
	shader->addUniformLocation("noiseSampler", 3);
	shader->addUniformLocation("cubemapSampler", 4);

	shader->addAttribute("vertexPosition",  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	shader->addAttribute("vertexUV",        2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	shader->addAttribute("vertexNormal",    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	shader->addAttribute("vertexTangent",   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	shader->addAttribute("vertexBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	//Allocate all textures for the interior
	if (mInterior.materialName.size()) {
		for (U32 i = 0; i < mInterior.materialName.size(); i ++) {
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
			material->setTexture(mNoiseTexture, GL_TEXTURE3);

			//TODO: Make this material-dependant when we get around to material properties
			material->setShader(shader);

			//Assign the texture
			mMaterialList.push_back(material);
		}
	}
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
			TriangleF triangle(mInterior.point[mInterior.index[surface.windingStart + j]], mInterior.point[mInterior.index[surface.windingStart + j + 1]], mInterior.point[mInterior.index[surface.windingStart + j + 2]]);

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

const glm::vec3 GameInterior::getPosition() const {
	return mActor->getPosition();
}

const glm::quat GameInterior::getRotation() const {
	return mActor->getRotation();
}

void GameInterior::setPosition(const glm::vec3 &pos) {
	mActor->setPosition(pos);
}

void GameInterior::setRotation(const glm::quat &rot) {
	mActor->setRotation(rot);
}

const glm::vec3 GameInterior::getScale() const {
	return mActor->getScale();
}
void GameInterior::setScale(const glm::vec3 &scale) {
	mActor->setScale(scale);
}