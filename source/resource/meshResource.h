//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_MESHRESOURCE_H_
#define _RESOURCE_MESHRESOURCE_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "resource/IResource.h"

class MeshResource : public IResource {
	typedef IResource Parent;
public:
	MeshResource(const std::string &file);
	~MeshResource();

	bool load();

private:
	std::string mResourceFile;
};

#endif // _RESOURCE_MESHRESOURCE_H_
