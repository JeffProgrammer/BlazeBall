//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
// Note: portions of this code is from the assimp sample code:
// https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c
// https://github.com/assimp/assimp/tree/master/samples/SimpleAssimpViewX
//------------------------------------------------------------------------------

#include "resource/interiorResource.h"

InteriorResource::InteriorResource(const std::string &file) {
	mResourceFile = file;
}

InteriorResource::~InteriorResource() {

}

bool InteriorResource::load() {
	return true;
}