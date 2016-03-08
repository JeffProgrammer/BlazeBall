//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "resource/materialResource.h"
#include "resource/resourceLoader.h"
#include "render/material.h"

MaterialResource::MaterialResource(const std::string &name, std::vector<std::pair<Texture*, GLuint>> &textures) {
	mName = name;
	mMaterial = new Material(name, textures);
}

MaterialResource::~MaterialResource() {
	delete mMaterial;
}

void MaterialResource::setShader(Shader *shader) {
	assert(mMaterial != nullptr);
	mMaterial->setShader(shader);
}

void MaterialResource::addMeshRef(MeshResource *resource) {
	// Make sure that the mesh hasn't already been added to the material.
	if (std::find(mMeshes.begin(), mMeshes.end(), resource) != mMeshes.end())
		return;

	mMeshes.push_back(resource);
}

void MaterialResource::removeMeshRef(MeshResource *resource) {
	// Make sure that the object is in the vector
	auto pos = std::find(mMeshes.begin(), mMeshes.end(), resource);
	if (pos != mMeshes.end())
		mMeshes.erase(pos);
}
