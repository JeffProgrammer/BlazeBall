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

#ifndef gameObject_h
#define gameObject_h

#include <glm/matrix.hpp>

#include "base/types.h"
#include "game/movement.h"
#include "render/renderInfo.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

class GameObject {
protected:
	glm::vec3 mOrigin;
	glm::quat mRotation;
	glm::vec3 mScale;
	
public:
	GameObject() : mOrigin(glm::vec3(0)), mRotation(glm::quat()), mScale(glm::vec3(1)) {};
	virtual ~GameObject() {};

	virtual bool isRenderable() { return false; }
	virtual bool isReflectable() { return false; }
	
	virtual glm::vec3 getPosition() { return mOrigin; }
	virtual void setPosition(const glm::vec3 &position) { mOrigin = position; }

	virtual glm::quat getRotation() { return mRotation; }
	virtual void setRotation(const glm::quat &rotation) { mRotation = rotation; }

	virtual glm::vec3 getScale() { return mScale; }
	virtual void setScale(const glm::vec3 &scale) { mScale = scale; }

	virtual void updateCamera(const Movement &movement, const F64 &deltaMS);
	virtual void updateMove(const Movement &movement, const F64 &deltaMS);
	virtual void getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos);

	virtual void updateTick(const F64 &deltaMS) = 0;
};

#endif
