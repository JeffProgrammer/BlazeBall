//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "resource/resourceLoader.h"
#include "resource/meshResource.h"

// For getcwd
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

ResourceLoader *ResourceLoader::sResourceLoader = nullptr;

void ResourceLoader::create() {
	sResourceLoader = new ResourceLoader();
}

void ResourceLoader::destroy() {
	delete sResourceLoader;
}

ResourceLoader::ResourceLoader() {
	char buffer[1024]; // If your path is longer than this, idk what to say.
	mCurrentWorkingDir = getcwd(buffer, 1024);
}

ResourceLoader::~ResourceLoader() {

}

IResource* ResourceLoader::loadMesh(const std::string &file) {
	// See if it's already in the map. If it is, we are good.
	if (containsResource(file))
		return mResourceMap[file];

	// Make sure that this file exists!
	if (!IO::isfile(file)) {
		IO::printf("Unable to find resource %s!\n", file.c_str());
		return nullptr;
	}

	MeshResource *resource = new MeshResource(file);
	if (resource->load()) {
		// If we loaded the resource successfully, then store the resource
		// pointer to our hash maps and then return the resource.
		mResourceMap[file] = resource;
		mMeshResourceMap[file] = resource;
		return static_cast<IResource*>(resource);
	}
	return nullptr;
}