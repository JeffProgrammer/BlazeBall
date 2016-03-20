//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#ifndef _RENDER_MATERIALMANAGER_H_
#define _RENDER_MATERIALMANAGER_H_

#include <stdio.h>
#include <vector>
#include "base/types.h"
#include "bitmap/texture.h"
#include "graphics/shader.h"
#include "render/material.h"

struct ShaderInfo;

#define MATERIALMGR MaterialManager::getSingleton()

class MaterialManager {
private:

	std::vector<Material*> mMaterialList;
	
public:
	static MaterialManager *getSingleton() {
		static MaterialManager *manager = nullptr;
		if (manager == nullptr)
			manager = new MaterialManager();
		return manager;
	}
	
	MaterialManager();
	~MaterialManager();
	
	Material *getMaterial(const std::string &name) const;
	
	bool isNull(const Material *material) const;
	
	void addMaterial(Material *material);
	
	void removeMaterial(Material *material);
	
	S32 indexof(Material *material) const;
};

#endif // _RENDER_MATERIALMANAGER_H_