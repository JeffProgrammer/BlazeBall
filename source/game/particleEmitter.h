//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PARTICLEEMITTER_H_
#define _GAME_PARTICLEEMITTER_H_

#include "game/gameObject.h"
#include "render/material.h"

#ifdef __APPLE__
#include <sys/time.h>
inline F64 getTime() {
	timeval tv;
	gettimeofday(&tv, NULL);
	long long time = tv.tv_usec + (tv.tv_sec * 1000000ULL);
	return static_cast<F64>(time) / 1000000.f;
}
#elif _WIN32
#include <windows.h>
inline F64 getTime() {
	LARGE_INTEGER time, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&time);

	//Convert to microseconds first so we don't lose precision
	time.QuadPart *= 1000000;
	time.QuadPart /= frequency.QuadPart;

	return static_cast<F64>(time.QuadPart) / 1000000.f;
}
#endif

struct Particle {
	static F64 start;
	glm::vec3 position;
	glm::vec3 velocity;
	F32 life;
	F32 creation;

	Particle() {
		if (start == 0) {
			start = getTime();
		}

		position = glm::vec3(0.f);
		velocity = glm::vec3(0.f);
		life = 0.0f;

		creation = getTime();
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
