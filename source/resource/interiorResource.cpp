//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "resource/interiorResource.h"

InteriorResource::InteriorResource(const std::string &file) : MeshResource(file) {

}

InteriorResource::~InteriorResource() {

}

bool InteriorResource::load() {
	return true;
}