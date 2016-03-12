//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_RESOURCELOADER_H_
#define _RESOURCE_RESOURCELOADER_H_

#include <unordered_map>
#include "base/types.h"
#include "resource/IResource.h"

class MeshResource;
class InteriorResource;
class ShapeResource;
class MaterialResource;
class ShaderResource;

class ResourceLoader {
private:
	static ResourceLoader *sResourceLoader;

	/**
	 * Holds a map of resources.
	 * std::string - The file path of the resource on disk.
	 * IResource*  - The resource object loaded into memory.
	 */
	std::unordered_map<std::string, IResource*> mResourceMap;

	std::unordered_map<std::string, MeshResource*> mMeshResourceMap;

	std::vector<MaterialResource*> mMaterialResources;

	// TEMPORARY
	std::unordered_map<Shader*, ShaderResource*> mShaderResources;

	std::string mCurrentWorkingDir;
public:
	static void create();
	static void destroy();
	static ResourceLoader *get() {
		assert(sResourceLoader != nullptr);
		return sResourceLoader;
	}

	ResourceLoader();
	~ResourceLoader();

	inline bool containsResource(const std::string &resource) {
		return (mResourceMap.find(resource) != mResourceMap.end());
	}

	inline ShaderResource* getShaderResource(Shader *shader) {
		if (mShaderResources.find(shader) == mShaderResources.end())
			return nullptr;
		return mShaderResources[shader];
	}

	inline const std::string getCurrentDirectory() const {
		return mCurrentWorkingDir;
	}

	ShapeResource* loadShape(const std::string &file);

	InteriorResource* loadInterior(const std::string &file);

	// Note: we should be loading shaders from json files. for now we
	// hardcode them in c++
	ShaderResource* loadShader(Shader *shader);

	void loadMaterials(const std::string &file);
};

#endif // _RESOURCE_RESOURCELOADER_H_