//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_MESHRESOURCE_H_
#define _RESOURCE_MESHRESOURCE_H_

#include "resource/IResource.h"
#include "render/shader.h"

class MaterialResource;

class ShaderResource : public IResource {
	typedef IResource Parent;
public:
	ShaderResource(const std::string &file);
	~ShaderResource();

	bool load();

	void addMaterial(MaterialResource *material);

	void removeMaterial(MaterialResource *material);

protected:
	std::string mResourceFile;
	Shader *mShader;

	/**
	 * The amount of materials that reference this shader.
	 */
	std::vector<MaterialResource*> mMaterialList;
};

#endif // _RESOURCE_MESHRESOURCE_H_
