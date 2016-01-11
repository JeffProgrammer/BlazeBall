//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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

Skybox::Skybox(World *world) : RenderedObject(world) {
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
	shader->setUniform("extent", 1000.f);
	shader->setUniformMatrix("viewMat", GL_FALSE, skyboxView);

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
	// faces 0-5
	AddFieldSimple("face0", std::string, offsetof(Skybox, mFace0));
	AddFieldSimple("face1", std::string, offsetof(Skybox, mFace1));
	AddFieldSimple("face2", std::string, offsetof(Skybox, mFace2));
	AddFieldSimple("face3", std::string, offsetof(Skybox, mFace3));
	AddFieldSimple("face4", std::string, offsetof(Skybox, mFace4));
	AddFieldSimple("face5", std::string, offsetof(Skybox, mFace5));
}

void Skybox::initScript(ScriptEngine *engine) {
	// Nothing.
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