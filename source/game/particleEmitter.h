//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PARTICLEEMITTER_H_
#define _GAME_PARTICLEEMITTER_H_

#include "game/gameObject.h"
#include "render/material.h"

struct Particle {
	glm::vec3 position;
	F32 life;

	Particle() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		life = 0.0f;
	}
};

static GLfloat sParticleVertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, 0.5f
};

class ParticleEmitter : public GameObject {
protected:
	std::vector<Particle> mParticles;

	GLuint mVertVBO;
	GLuint mVBO;

	Material *mMaterial;

public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	virtual void loadMatrix(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, glm::mat4 &modelMatrix);
	virtual void render(Shader *shader, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix);
	
	virtual void updateTick(const F64 &deltaMS);
};

#endif // _GAME_PARTICLEEMITTER_H_
