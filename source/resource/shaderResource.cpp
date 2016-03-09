//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "resource/shaderResource.h"

ShaderResource::ShaderResource(const std::string &file) {
	mResourceFile = file;
	mShader = nullptr;
}

ShaderResource::~ShaderResource() {
	if (mShader)
		delete mShader;
}

bool ShaderResource::load() {
	return true; // TODO
}

void ShaderResource::addMaterial(MaterialResource *material) {
	// Only add the material once!
	if (std::find(mMaterialList.begin(), mMaterialList.end(), material) == mMaterialList.end())
		mMaterialList.push_back(material);
	else
		IO::printf("ShaderResource :: can only add the material %p once to the cache.\n", material);
}

void ShaderResource::removeMaterial(MaterialResource *material) {
	// Make sure that the material exists!
	auto pos = std::find(mMaterialList.begin(), mMaterialList.end(), material);
	if (pos != mMaterialList.end())
		mMaterialList.erase(pos);
	else
		IO::printf("ShaderResource :: unable to remove material %p! It can't be found!\n", material);
}