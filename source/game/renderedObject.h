//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
	virtual void render(RenderInfo &info) = 0;
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
