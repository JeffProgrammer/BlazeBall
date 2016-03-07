//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "resource/materialResource.h"
#include "resource/resourceLoader.h"

MaterialResource::MaterialResource(const std::string &file) {
	mResourceFile = file;
	mMaterial = nullptr;
}

MaterialResource::~MaterialResource() {
	delete mMaterial;
}

bool MaterialResource::load() {
	mMaterial = new Material(mResourceFile);
	// TODO set shader reference to material.
	return true;
}