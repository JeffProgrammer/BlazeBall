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

#ifndef RenderedObject_h
#define RenderedObject_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gameObject.h"
#include "render/shader.h"
#include "render/renderInfo.h"

class RenderedObject : public GameObject {
public:
	/**
	 * Render the object using OpenGL.
	 * @param info A RenderInfo struct containing all the render state information.
	 */
	virtual void render(const RenderInfo &info) = 0;
	/**
	 * If the object should be rendered.
	 * @return True for rendered objects
	 */
	virtual bool isRenderable() { return true; }

	/**
	 * Load the object's model matrix into a mat4 reference.
	 * @param info        A RenderInfo struct with all the render state information.
	 * @param modelMatrix A reference to a matrix into which the model matrix will be loaded.
	 */
	virtual void calculateModelMatrix(const RenderInfo &info, glm::mat4 &modelMatrix);
	/**
	 * Send the object's model matrix to OpenGL
	 * @param info   A RenderInfo struct with all the render state information.
	 * @param shader The shader which will be used for finding uniform locations.
	 */
	void loadModelMatrix(const RenderInfo &info, Shader *shader);
};

#endif
