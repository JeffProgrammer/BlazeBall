//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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

#include "render/material.h"
#include "base/io.h"

Material::~Material() {

}

void Material::loadTextures(const std::string &path) {
	//Don't try to load a texture if we don't have one
	if (path.length() == 0)
		return;

	//Some basic fields for identifying the material
	this->name = IO::getName(path, '/');
	this->path = path;

	//Normal and specular maps are .normal.png and .spec.png
	std::string norm = path + ".normal.png";
	std::string spec = path + ".alpha.png";

	//Try to load these textures
	loadTextures(path, norm, spec);
}

void Material::loadTextures(const std::string &diffusePath, const std::string &normalPath, const std::string &specularPath) {
	//Some basic fields for identifying the material in case we don't call the above method
	this->name = IO::getName(diffusePath, '/');
	this->path = diffusePath;

	//Diffuse texture- needs to be loaded or else we can't have a material.
	if (!tryLoadTexture(diffusePath, GL_TEXTURE0)) {
		printf("Could not load diffuse texture %s\n", diffusePath.c_str());
		return;
	}
	//Normal texture
	if (!tryLoadTexture(normalPath, GL_TEXTURE1)) {
		printf("Could not load normal texture %s\n", normalPath.c_str());

		//Attempt to load the default normal texture so we at least have something
		if (!tryLoadTexture(DEFAULT_NORMAL_TEXTURE, GL_TEXTURE1)) {
			printf("Could not load default normal texture!\n");
		}
	}
	//Specular texture
	if (!tryLoadTexture(specularPath, GL_TEXTURE2)) {
		printf("Could not load specular texture %s\n", specularPath.c_str());

		//Attempt to load the default specular texture so we at least have something
		if (!tryLoadTexture(DEFAULT_SPECULAR_TEXTURE, GL_TEXTURE2)) {
			printf("Could not load default specular texture!\n");
		}
	}
}

bool Material::tryLoadTexture(const std::string &path, GLuint index) {
	//See if we have a previous texture so we can delete it
	Texture *previous = getTexture(index);

	//Try to load the bitmap at the given path
	Texture *loaded = IO::loadTexture(path);
	if (loaded) {
		loaded->setTexNum(index);

		//If we have a new texture, trash the old one.
		if (previous) {
			delete previous;
		}
		//Update our texture to use this new one
		setTexture(loaded, index);
		return true;
	}
	//Either couldn't load the bitmap or the object...
	return false;
}

void Material::activate() {
	//If this material has a shader, we should use it.
//	if (shaderInfo && shaderInfo->shader) {
//		shaderInfo->shader->activate();
//	}
	//Activate all of the textures on this shader.
	for (auto iter : textures) {
		if (iter.second) {
			if (!iter.second->generated) {
				iter.second->generateBuffer();
			}

			iter.second->activate();
		}
	}
}

void Material::deactivate() {
	//If this material has a shader, we need to deactivate it
//	if (shaderInfo && shaderInfo->shader) {
//		shaderInfo->shader->deactivate();
//	}
	//Deactivate all of the textures on this shader.
	for (auto iter : textures) {
		if (iter.second) {
			iter.second->deactivate();
		}
	}
}

void Material::setTexture(Texture *texture, GLuint index) {
	this->textures[index] = texture;
}
