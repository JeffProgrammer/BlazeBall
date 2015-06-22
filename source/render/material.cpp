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

Material::Material() : shader(nullptr), diffuse(nullptr), normal(nullptr), specular(nullptr), name(new String("")), path(new String("")) {

}

Material::~Material() {

}

void Material::loadDiffuse(String path) {
	this->name = new String(io->getName(path));
	this->path = new String(path);

	if ((diffuse = io->loadTexture(path)) == nullptr) {
		fprintf(stderr, "Error in reading bitmap: %s Other error\n", (char *)path);
	} else {
		//So we set the texNum
		setDiffuseTex(diffuse);
	}
}

void Material::generate() {
	if (diffuse) {
		if (!diffuse->generated) {
			diffuse->generateBuffer();
		}
	}
	if (normal) {
		if (!normal->generated) {
			normal->generateBuffer();
		}
	}
	if (specular) {
		if (!specular->generated) {
			specular->generateBuffer();
		}
	}
}

void Material::activate() {
	if (shader) {
		shader->activate();
	}
	if (diffuse) {
		if (!diffuse->generated) {
			diffuse->generateBuffer();
		}
		diffuse->activate();
	}
	if (normal) {
		if (!normal->generated) {
			normal->generateBuffer();
		}
		normal->activate();
	}
	if (specular) {
		if (!specular->generated) {
			specular->generateBuffer();
		}
		specular->activate();
	}
}

void Material::deactivate() {
	if (shader) {
		shader->deactivate();
	}
	if (diffuse) {
		diffuse->deactivate();
	}
	if (normal) {
		normal->deactivate();
	}
	if (specular) {
		specular->deactivate();
	}
}

void Material::setDiffuseTex(Texture *texture) {
	this->diffuse = texture;
	this->diffuse->setTexNum(GL_TEXTURE0);
}

void Material::setNormalTex(Texture *texture) {
	this->normal = texture;
	this->normal->setTexNum(GL_TEXTURE1);
}

void Material::setSpecularTex(Texture *texture) {
	this->specular = texture;
	this->specular->setTexNum(GL_TEXTURE2);
}