//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "resource/materialResource.h"
#include "resource/resourceLoader.h"
#include "render/material.h"

MaterialResource::MaterialResource(const std::string &file) {
	mDiffuseTexture = file;
	mMaterial = nullptr;
}

MaterialResource::~MaterialResource() {

}

bool MaterialResource::load() {
	mMaterial = std::make_unique<Material>(mDiffuseTexture);
	return true;
}

void MaterialResource::setShader(Shader *shader) {
	assert(mMaterial != nullptr);
	mMaterial->setShader(shader);
}

void MaterialResource::addMeshRef(MeshResource *resource) {
	mMeshses.push_back(resource);
}

void MaterialResource::removeMeshRef(MeshResource *resource) {

}
