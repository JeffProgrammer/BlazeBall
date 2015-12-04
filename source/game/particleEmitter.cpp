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

// special thanks to this dood (WTFPL)
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
//
// NOTE: this class uses hardware instancing which is not part of the OpenGL 2.1
//       specification (Instancing is part of 3.3 core profile).
//
// Supported hardware and OS with this extension:
//   Intel HD Graphics+
//   GeForce 8000+
//   AMD Radeon HD 2000+
//
//   Windows XP+
//   Mac OSX 10.6.8+
//   Linux
//
// for more information, see this opengl database:
// http://delphigl.de/glcapsviewer/gl_listreports.php?listreportsbyextension=GL_ARB_instanced_arrays

#include "game/particleEmitter.h"
#include "render/util.h"
#include <glm/gtc/matrix_transform.hpp>

#define MAX_PARTICLE_COUNT 100
#define PARTICLE_TIME 30000.0

// TODO: make particles render in a particle manager, so that we only have to have
//       one buffer for the particle vertex geometry.

F64 Particle::start = 0;

ParticleEmitter::ParticleEmitter() : RenderedObject() {

	// create particles
	for (S32 i = 0; i < MAX_PARTICLE_COUNT; i++) {
		Particle p;
		p.position = getPosition();
		//Temp
#define RAND_VEL 1.f - (((F32)rand() / (F32)RAND_MAX) * 2.f)
		p.velocity = glm::vec3(RAND_VEL, RAND_VEL, RAND_VEL);
#undef RAND_VEL
		p.life = PARTICLE_TIME;

		mParticles.push_back(p);
	}

	mMaterial = NULL;

	// billboard data does not change
	glGenBuffers(1, &mVertVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, sParticleVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLE_COUNT, &mParticles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleEmitter::~ParticleEmitter() {
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mVertVBO);
}

void ParticleEmitter::render(RenderInfo &info) {
	if (mMaterial == NULL)
		return;

	glDepthMask(GL_FALSE);

	mMaterial->activate();
	Shader *shader = mMaterial->getShader();
	info.loadShader(shader);

	// get our view matrix as this is used for rendering as a billboard
	glm::mat4 view = glm::mat4(glm::mat3(info.viewMatrix));
	view = glm::inverse(view);
	glUniformMatrix4fv(shader->getUniformLocation("inverseCameraMatrix"), 1, GL_FALSE, &view[0][0]);
	GL_CHECKERRORS();

	// bind the vertices that we'll be instancing
	glBindBuffer(GL_ARRAY_BUFFER, mVertVBO);
	shader->enableAttribute("vertex", 2, GL_FLOAT, GL_FALSE, 0, 0);

	// let's use positions
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	shader->enableAttribute("position", 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, position));
	shader->enableAttribute("velocity", 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, velocity));
	shader->enableAttribute("creation", 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, creation));

	glUniform1f(shader->getUniformLocation("currentTime"), getTime() - Particle::start);

	// let opengl know where to divide up the data for each instance
	glVertexAttribDivisor(shader->getAttributeLocation("vertex"), 0); // use all 4 vertices per strip
	glVertexAttribDivisor(shader->getAttributeLocation("position"), 1); // 1 position per strip
	glVertexAttribDivisor(shader->getAttributeLocation("velocity"), 1); // 1 position per strip
	glVertexAttribDivisor(shader->getAttributeLocation("creation"), 1); // 1 position per strip
	GL_CHECKERRORS();

	// draw each particle
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MAX_PARTICLE_COUNT);
	GL_CHECKERRORS();

	// reset everything.
	shader->disableAttribute("vertex");
	shader->disableAttribute("position");
	mMaterial->deactivate();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(shader->getAttributeLocation("vertex"), 0);
	glVertexAttribDivisor(shader->getAttributeLocation("position"), 0);
	glVertexAttribDivisor(shader->getAttributeLocation("velocity"), 0);
	glVertexAttribDivisor(shader->getAttributeLocation("creation"), 0);
	GL_CHECKERRORS();

	glDepthMask(GL_TRUE);
}

void ParticleEmitter::updateTick(const F64 &deltaMS) {
	if (deltaMS == 0.0)
		return;

	const F32 t = static_cast<F32>(deltaMS);

	for (Particle &particle : mParticles) {
		particle.life -= static_cast<F32>(deltaMS);
		
		// give it some movement
		particle.position += particle.velocity * (t / 1000.f);

		// if particle ran out of time, make it respawn
		if (particle.life <= 0.0f) {
			particle.life = PARTICLE_TIME;
			particle.position = getPosition();
		}
	}
}