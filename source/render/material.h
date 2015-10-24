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

#ifndef material_h
#define material_h

#include <stdio.h>
#include <unordered_map>
#include "base/types.h"
#include "texture/texture.h"
#include "render/shader.h"

#define DEFAULT_NORMAL_TEXTURE   "DefaultNormal.png"
#define DEFAULT_SPECULAR_TEXTURE "DefaultSpec.png"

class Material {
	std::unordered_map<GLuint, Texture *> textures;
	Shader *shader;

	std::string name;
	std::string path;

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
public:
	Material(const std::string &name, Texture *texture, GLuint location) : shader(nullptr), name(name) {
		textures[location] = texture;
	}

	/**
	 * Construct a material from only a diffuse texture path, attempting to resolve
	 * specular and normal map paths from the diffuse texture.
	 * @param path The path for the material's diffuse texture.
	 */
	Material(const std::string &path) : shader(nullptr) {
		loadTextures(path);
	}
	
	/**
	 * Construct a material from a diffuse, normal, and specular texture path.
	 * @param diffusePath The path for the material's diffuse texture.
	 * @param normalPath The path for the material's normal texture.
	 * @param specularPath The path for the material's specular texture.
	 */
	Material(const std::string &diffusePath, const std::string &normalPath, const std::string &specularPath) : shader(nullptr) {
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
		return this->textures[index];
	}
	
	/**
	 * Get the material's shader
	 * @return The material's shader
	 */
	Shader *getShader() const {
		if (this->shader == nullptr)
			return Shader::defaultShader;
		return this->shader;
	}
	
	/**
	 * Get the material's texture path (by default the diffuse texture)
	 * @return The material's texture path
	 */
	std::string getPath() const {
		return this->path;
	}
	
	/**
	 * Gets the material's name
	 * @return The material's name
	 */
	std::string getName() const {
		return this->name;
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
		if (shader == nullptr)
			shader = Shader::defaultShader;
		this->shader = shader;
	}

	/**
	 * Activate the texture, and its shader if one is defined.
	 */
	void activate();
	
	/**
	 * Deactivate the texture, and its shader if one is defined.
	 */
	void deactivate();
};

#endif 