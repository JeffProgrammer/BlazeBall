//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/gameInterior.h"
#include "base/io.h"
#include "physics/physicsBody.h"
#include "render/texture/texture.h"
#include <string>
#include <cfloat>

IMPLEMENT_SCRIPTOBJECT(GameInterior, RenderedObject);

void GameInterior::initFields() {
	AddFieldSimple("interiorFile", std::string, offsetof(GameInterior, mInteriorFile));
}

GameInterior::GameInterior(World *world) : RenderedObject(world), mActor(nullptr) {

}

GameInterior::~GameInterior() {
	glDeleteBuffers(1, &mVbo);
}

void GameInterior::onAddToScene() {
	Parent::onAddToScene();

	// create the interior
	// TODO: create a resource cache for difs
	DIF::DIF dif;

	std::string path = mInteriorFile.c_str();
	std::string directory = IO::getPath(path);
	std::ifstream file(path, std::ios::binary);

	if (!dif.read(file)) {
		IO::printf("DIF file could not be read!\n");
		return;
	}
		
	// load interior
	mInterior = dif.interior[0];
	generateMaterials(directory);

	// make mesh
	generateMesh();

	// cleanup
	file.close();

	// init graphics
	gfxInit();
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
	mActor = mWorld->getPhysicsEngine()->createInterior(this);
	mActor->setWorld(mWorld);
	mWorld->getPhysicsEngine()->addBody(mActor);
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

Vec3 GameInterior::getPosition() const {
	return mActor->getPosition();
}

Quat GameInterior::getRotation() const {
	return mActor->getRotation();
}

Vec3 GameInterior::getScale() const {
	return mActor->getScale();
}

void GameInterior::setPosition(const Vec3 &pos) {
	if (mActor)
		mActor->setPosition(pos);
}

void GameInterior::setRotation(const Quat &rot) {
	if (mActor)
		mActor->setRotation(rot);
}

void GameInterior::setScale(const Vec3 &scale) {
	if (mActor)
		mActor->setScale(scale);
}

bool GameInterior::read(CharStream &stream) {
	if (!GameObject::read(stream)) {
		return false;
	}

	mInteriorFile = stream.pop<std::string>();
	return true;
}
bool GameInterior::write(CharStream &stream) const {
	if (!GameObject::write(stream)) {
		return false;
	}

	stream.push<std::string>(mInteriorFile);
	return true;
}

void GameInterior::initScript(ScriptEngine *engine) {
	// Nothing.
}