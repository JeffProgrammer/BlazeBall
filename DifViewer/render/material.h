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

#ifndef material_h
#define material_h

#include <stdio.h>
#include "types.h"
#include "texture.h"
#include "shader.h"

class Material {
	Texture *diffuse;
	Texture *normal;
	Texture *specular;
	Shader *shader;

	String name;
	String path;
public:
	Material();
	~Material();

	Material(String path) : diffuse(nullptr), normal(nullptr), specular(nullptr), shader(nullptr) {
		loadDiffuse(path);
	}

	Texture *getDiffuseTex() {
		return this->diffuse;
	}
	Texture *getNormalTex() {
		return this->normal;
	}
	Texture *getSpecularTex() {
		return this->specular;
	}
	Shader *getShader() {
		return this->shader;
	}

	void setDiffuseTex(Texture *texture) {
		this->diffuse = texture;
	}
	void setNormalTex(Texture *texture) {
		this->normal = texture;
	}
	void setSpecularTex(Texture *texture) {
		this->specular = texture;
	}
	void setShader(Shader *shader) {
		this->shader = shader;
	}

	void loadDiffuse(String path);
	void generate();
	void activate();
	void deactivate();
};

#endif 