//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/material.h"
#include "base/io.h"

IMPLEMENT_SCRIPTOBJECT(Material, ScriptObject);

Material::~Material() {

}

void Material::loadTextures(const std::string &path) {
	//Don't try to load a texture if we don't have one
	if (path.length() == 0)
		return;

	//Some basic fields for identifying the material
	mName = IO::getName(path);
	mPath = path;

	//Normal and specular maps are .normal.png and .spec.png
	std::string norm = path + ".normal.png";
	std::string spec = path + ".alpha.png";

	//Try to load these textures
	loadTextures(path, norm, spec);
}

void Material::loadTextures(const std::string &diffusePath, const std::string &normalPath, const std::string &specularPath) {
	//Some basic fields for identifying the material in case we don't call the above method
	mName = IO::getName(diffusePath);
	mPath = diffusePath;

	//Diffuse texture- needs to be loaded or else we can't have a material.
	if (!tryLoadTexture(diffusePath, GL_TEXTURE0)) {
		IO::printf("Could not load diffuse texture %s\n", diffusePath.c_str());
		return;
	}
	//Normal texture
	if (!tryLoadTexture(normalPath, GL_TEXTURE1)) {
		IO::printf("Could not load normal texture %s\n", normalPath.c_str());

		//Attempt to load the default normal texture so we at least have something
		if (!tryLoadTexture(DEFAULT_NORMAL_TEXTURE, GL_TEXTURE1)) {
			IO::printf("Could not load default normal texture!\n");
		}
	}
	//Specular texture
	if (!tryLoadTexture(specularPath, GL_TEXTURE2)) {
		IO::printf("Could not load specular texture %s\n", specularPath.c_str());

		//Attempt to load the default specular texture so we at least have something
		if (!tryLoadTexture(DEFAULT_SPECULAR_TEXTURE, GL_TEXTURE2)) {
			IO::printf("Could not load default specular texture!\n");
		}
	}
}

bool Material::tryLoadTexture(const std::string &path, const GLuint &index) {
	//See if we have a previous texture so we can delete it
	Texture *previous = getTexture(index);

	//Try to load the bitmap at the given path
	Texture *loaded = IO::loadTexture(path);
	if (loaded) {
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
	if (mShader) {
		mShader->activate();
	}
	//Activate all of the textures on this shader.
	for (auto iter : mTextures) {
		if (iter.second) {
			iter.second->activate(iter.first);
		}
	}
}

void Material::deactivate() {
	//If this material has a shader, we need to deactivate it
	if (mShader) {
		mShader->deactivate();
	}
	//Deactivate all of the textures on this shader.
	for (auto iter : mTextures) {
		if (iter.second) {
			iter.second->deactivate();
		}
	}
}

void Material::setTexture(Texture *texture, const GLuint &index) {
	this->mTextures[index] = texture;
}

void Material::initFields() {
	AddFieldSimple("shader", std::string, offsetof(Material, mShaderName));
	AddFieldSimple("diffuseTexture", std::string, offsetof(Material, mDiffuseTextureName));
	AddFieldSimple("normalTexture", std::string, offsetof(Material, mNormalTextureName));
	AddFieldSimple("specularTexture", std::string, offsetof(Material, mSpecularTextureName));
}