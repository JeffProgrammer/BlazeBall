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

#ifndef _RENDER_MATERIAL_H_
#define _RENDER_MATERIAL_H_

#include <stdio.h>
#include <unordered_map>
#include "base/types.h"
#include "texture/texture.h"
#include "render/shader.h"
#include "scriptEngine/concreteClassRep.h"
#include "game/scriptObject.h"

#define DEFAULT_NORMAL_TEXTURE   "DefaultNormal.png"
#define DEFAULT_SPECULAR_TEXTURE "DefaultSpec.png"

class Material : public ScriptObject {
	std::unordered_map<GLuint, Texture *> mTextures;
	Shader *mShader;

	//std::string mName;
	std::string mPath;

	std::string mDiffuseTextureName;
	std::string mNormalTextureName;
	std::string mSpecularTextureName;
	std::string mShaderName;

	std::string mMaterialName;

	/**
	 * Attempt to load textures for the material from a given diffuse path, resolving
	 * normal and specular texures.
	 * @param path The path for the material's diffuse texture.
	 */
	void loadTextures(const std::string &path);
	/**
	 * Attempt to load textures for the material from a diffuse, normal, and specular
	 * file path.
	 * @param diffusePath The path for the material's diffuse texture.
	 * @param normalPath The path for the material's normal texture.
	 * @param specularPath The path for the material's specular texture.
	 */
	void loadTextures(const std::string &diffusePath, const std::string &normalPath, const std::string &specularPath);
	/**
	 * Attempt to load a single texture into a given index
	 * @param path The path of the texture to load
	 * @param index The index where the texture will be assigned
	 * @return If the texture was loaded successfully.
	 */
	bool tryLoadTexture(const std::string &path, const GLuint &index);

	DECLARE_SCRIPTOBJECT(Material);
public:
	Material() {
		// don't use this yet. this is for script exposure. TODO
	}

	/**
	 * Construct a material from only a name and texture. No files will be loaded.
	 * @param name     The material's name
	 * @param texture  A texture that will be used on the material
	 * @param location The location for the given texture
	 */
	Material(const std::string &name, Texture *texture, GLuint location) : mShader(nullptr), mMaterialName(name) {
		setTexture(texture, location);
	}

	/**
	 * Construct a material using a name and a list of textures. No files will be loaded.
	 * @param name     The material's name
	 * @param textures A vector of <Texture *, GLuint> pairs specifying the textures
	 *                 and their desired locations.
	 */
	Material(const std::string &name, const std::vector<std::pair<Texture *, GLuint>> &textures) : mShader(nullptr), mMaterialName(name) {
		for (auto &texture : textures) {
			setTexture(texture.first, texture.second);
		}
	}

	/**
	 * Construct a material from only a diffuse texture path, attempting to resolve
	 * specular and normal map paths from the diffuse texture. The material's name
	 * will be resolved from the file path.
	 * @param path The path for the material's diffuse texture.
	 */
	Material(const std::string &path) : mShader(nullptr) {
		loadTextures(path);
	}

	/**
	 * Construct a material from only a shader. This material will have no textures.
	 * @param name   The material's name
	 * @param shader The shader to be used with this material
	 */
	Material(const std::string &name, Shader *shader) : mShader(shader), mMaterialName(name) {
	}
	
	/**
	 * Construct a material from a diffuse, normal, and specular texture path.
	 * The material's name will be resolved from the file path.
	 * @param diffusePath The path for the material's diffuse texture.
	 * @param normalPath The path for the material's normal texture.
	 * @param specularPath The path for the material's specular texture.
	 */
	Material(const std::string &diffusePath, const std::string &normalPath, const std::string &specularPath) : mShader(nullptr) {
		loadTextures(diffusePath, normalPath, specularPath);
	}
	
	/**
	 * Destroy the material, releasing its textures
	 */
	~Material();

	/**
	 * Get the texture that the material has assigned for a given index
	 * @param index The OpenGL texture index
	 * @return The currently assigned texture at that index
	 */
	Texture *getTexture(const GLuint &index) {
		return this->mTextures[index];
	}
	
	/**
	 * Get the material's shader
	 * @return The material's shader
	 */
	Shader *getShader() const {
		return this->mShader;
	}
	
	/**
	 * Get the material's texture path (by default the diffuse texture)
	 * @return The material's texture path
	 */
	std::string getPath() const {
		return this->mPath;
	}
	
	/**
	 * Gets the material's name
	 * @return The material's name
	 */
	std::string getName() const {
		return this->mMaterialName;
	}

	/**
	 * Assign a texture on the material at a given index
	 * @param texture The texture to assign
	 * @param index The desired index for that texture
	 */
	void setTexture(Texture *texture, const GLuint &index);
	
	/**
	 * Set the material's shade
	 * @param shader the new shader for the material
	 */
	void setShader(Shader *shader) {
		this->mShader = shader;
	}

	/**
	 * Activate the texture, and its shader if one is defined.
	 */
	void activate();
	
	/**
	 * Deactivate the texture, and its shader if one is defined.
	 */
	void deactivate();

	static void initFields();
};

#endif // _RENDER_MATERIAL_H_