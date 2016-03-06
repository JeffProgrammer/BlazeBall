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

#ifndef _RENDER_RESOURCELOADER_H_
#define _RENDER_RESOURCELOADER_H_

#include <unordered_map>
#include "base/types.h"
#include "resource/IResource.h"
#include "resource/meshResource.h"

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
public:
	static void create();
	static void destroy();

	ResourceLoader();
	~ResourceLoader();

	inline bool containsResource(const std::string &resource) {
		return (mResourceMap.find(resource) != mResourceMap.end());
	}

	IResource* loadMesh(const std::string &file);
};

#endif // _RENDER_RESOURCELOADER_H_