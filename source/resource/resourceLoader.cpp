//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include "resource/resourceLoader.h"

ResourceLoader *ResourceLoader::sResourceLoader = nullptr;

void ResourceLoader::create() {
	sResourceLoader = new ResourceLoader();
}

void ResourceLoader::destroy() {
	delete sResourceLoader;
}

ResourceLoader::ResourceLoader() {

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