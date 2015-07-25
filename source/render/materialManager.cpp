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

#include "render/materialManager.h"

MaterialManager::MaterialManager() {
	
}

MaterialManager::~MaterialManager() {
	// destroy materials
	for (auto material : mMaterialList)
		delete material;
}

Material* MaterialManager::getMaterial(const std::string &name) const {
	// find material with the name provided
	for (auto material : mMaterialList) {
		if (material->getName() == name)
			return material;
	}
	return nullptr;
}

bool MaterialManager::containsMaterial(const std::string &name) const {
	for (auto material : mMaterialList) {
		if (material->getName() == name)
			return true;
	}
	return false;
}

void MaterialManager::addMaterial(Material *material) {
	mMaterialList.push_back(material);
}

void MaterialManager::removeMaterial(Material *material) {
	// get the index of the material
	S32 indexOf = indexof(material);
	if (indexOf != -1) {
		delete mMaterialList[indexOf];
		mMaterialList.erase(mMaterialList.begin() + indexOf);
	}
}

S32 MaterialManager::indexof(Material *material) const {
	S32 index = 0;
	for (auto mat : mMaterialList) {
		if (mat == material)
			return index;
		index++;
	}
	return -1;
}