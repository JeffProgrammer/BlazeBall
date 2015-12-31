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

IMPLEMENT_SCRIPTOBJECT(GameInterior, RenderedObject);

void GameInterior::initFields() {
	// TODO
}

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
			material->setShader(Shader::getShaderByName("Interior"));

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

glm::vec3 GameInterior::getPosition() {
	return mActor->getPosition();
}

glm::quat GameInterior::getRotation() {
	return mActor->getRotation();
}

void GameInterior::setPosition(const glm::vec3 &pos) {
	mActor->setPosition(pos);
}

void GameInterior::setRotation(const glm::quat &rot) {
	mActor->setRotation(rot);
}

glm::vec3 GameInterior::getScale() {
	return mActor->getScale();
}
void GameInterior::setScale(const glm::vec3 &scale) {
	mActor->setScale(scale);
}