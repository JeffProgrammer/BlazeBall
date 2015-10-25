//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "skybox.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

Skybox::Skybox(Material *material) : mGenerated(false), mMaterial(material) {

}

Skybox::~Skybox() {
	if (mGenerated) {
		glDeleteBuffers(1, &mBuffer);
	}
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

