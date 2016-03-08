//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_MATERIALRESOURCE_H_
#define _RESOURCE_MATERIALRESOURCE_H_

#include <vector>
#include <memory>
#include "resource/IResource.h"

class Material;
class MeshResource;

class MaterialResource : public IResource {
	typedef IResource Parent;
public:
	MaterialResource(const std::string &file);
	~MaterialResource();

	bool load();

	void setShader(Shader *shader);

	void addMeshRef(MeshResource *resource);

	void removeMeshRef(MeshResource *resource);

	inline Material *getMaterial() const {
		return mMaterial;
	}

private:
	std::vector<MeshResource*> mMeshes;
	
	std::string mResourceFile;

	Material *mMaterial;
};

#endif // _RESOURCE_MATERIALRESOURCE_H_
