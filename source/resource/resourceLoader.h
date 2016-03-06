//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_RESOURCELOADER_H_
#define _RENDER_RESOURCELOADER_H_

#include <unordered_map>
#include "base/types.h"
#include "resource/IResource.h"

class MeshResource;

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

	inline const std::string getCurrentDirectory() const {
		return mCurrentWorkingDir;
	}

	IResource* loadMesh(const std::string &file);
};

#endif // _RENDER_RESOURCELOADER_H_