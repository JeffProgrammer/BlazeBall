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

#include "skybox.h"
#include "render/texture/cubeMapFramebufferTexture.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_SCRIPTOBJECT(Skybox, RenderedObject);

static GLfloat sVertices[] = {
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};
static U32 sVertCount = sizeof(sVertices) / sizeof(GLfloat);

Skybox::Skybox() : RenderedObject() {
	mGenerated = false;
	mBuffer = 0;
	mMaterial = nullptr;
}

Skybox::~Skybox() {
	if (mGenerated) {
		glDeleteBuffers(1, &mBuffer);
	}
	delete mMaterial;
}

void Skybox::generate() {
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);

	//Just use the contents of sVertices
	glBufferData(GL_ARRAY_BUFFER, sVertCount * sizeof(GLfloat), sVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mGenerated = true;
}

void Skybox::render(RenderInfo &info) {
	if (!mGenerated) {
		generate();
	}

	glDepthFunc(GL_LEQUAL);

	Shader *shader = mMaterial->getShader();
	mMaterial->activate();
	info.loadShader(shader);

	//Strip any positional data from the camera, so we just have rotation
	glm::mat4 skyboxView = glm::mat4(glm::mat3(info.viewMatrix));
	shader->setUniform("inExtent", 1000.f);
	shader->setUniformMatrix(UNIFORM_VIEW_MATRIX_NAME, GL_FALSE, skyboxView);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	shader->enableAttributes();

	//Simple draw
	glDrawArrays(GL_TRIANGLES, 0, sVertCount);

	shader->disableAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	mMaterial->deactivate();

	glDepthFunc(GL_LESS);
}

void Skybox::onAddToScene() {
	Parent::onAddToScene();

	std::vector<CubeMapTexture::TextureInfo> textures;
	textures.push_back(CubeMapTexture::TextureInfo(mFace0, CubeMapTexture::PositiveX));
	textures.push_back(CubeMapTexture::TextureInfo(mFace1, CubeMapTexture::NegativeX));
	textures.push_back(CubeMapTexture::TextureInfo(mFace2, CubeMapTexture::PositiveY));
	textures.push_back(CubeMapTexture::TextureInfo(mFace3, CubeMapTexture::NegativeY));
	textures.push_back(CubeMapTexture::TextureInfo(mFace4, CubeMapTexture::PositiveZ));
	textures.push_back(CubeMapTexture::TextureInfo(mFace5, CubeMapTexture::NegativeZ));

	CubeMapTexture *cubeMap = new CubeMapTexture(textures);
	Material *skyMaterial = new Material("skybox", cubeMap, GL_TEXTURE0);
	skyMaterial->setShader(Shader::getShaderByName("Skybox"));

	// store material.
	mMaterial = skyMaterial;
}

void Skybox::initFields() {
	AddField(Skybox::mFace0, "face0");
	AddField(Skybox::mFace1, "face1");
	AddField(Skybox::mFace2, "face2");
	AddField(Skybox::mFace3, "face3");
	AddField(Skybox::mFace4, "face4");
	AddField(Skybox::mFace5, "face5");
}

bool Skybox::read(CharStream &stream) {
	if (!GameObject::read(stream)) {
		return false;
	}
	mFace0 = stream.pop<std::string>();
	mFace1 = stream.pop<std::string>();
	mFace2 = stream.pop<std::string>();
	mFace3 = stream.pop<std::string>();
	mFace4 = stream.pop<std::string>();
	mFace5 = stream.pop<std::string>();

	return true;
}
bool Skybox::write(CharStream &stream) const {
	if (!GameObject::write(stream)) {
		return false;
	}
	stream.push<std::string>(mFace0);
	stream.push<std::string>(mFace1);
	stream.push<std::string>(mFace2);
	stream.push<std::string>(mFace3);
	stream.push<std::string>(mFace4);
	stream.push<std::string>(mFace5);

	return true	;
}