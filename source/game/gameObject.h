//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
	/**
	 * The position of the GameObject.
	 */
	glm::vec3 mOrigin;

	/**
	 * The rotation of the GameObject.
	 */
	glm::quat mRotation;

	/**
	 * The scale of the GameObject.
	 */
	glm::vec3 mScale;
	
public:
	GameObject() : mOrigin(glm::vec3(0)), mRotation(glm::quat()), mScale(glm::vec3(1)) {};
	virtual ~GameObject() {};

	/**
	 * Checks to see if the GameObject is renderable.
	 * @return true If the GameObject renders, false otherwise.
	 */
	virtual bool isRenderable() { return false; }

	/**
	 * Checks to see if the GameObject can be placed into the reflection pass.
	 * @return true if the GameObject can be in the reflection pass, false otherwise.
	 */
	virtual bool isReflectable() { return false; }
	
	/**
	 * Gets the position of the GameObject.
	 * @return the position vector of the GameObject.
	 */
	virtual glm::vec3 getPosition() { return mOrigin; }

	/**
	 * Sets the position of the GameObject.
	 * @param position The position to set the GameObject.
	 */
	virtual void setPosition(const glm::vec3 &position) { mOrigin = position; }

	/**
	 * Gets the rotation of the GameObject as a quaternion.
	 * @return the rotation quaternion of the GameObject.
	 */
	virtual glm::quat getRotation() { return mRotation; }

	/**
	 * Sets the rotation of the GameObject as a quaternion.
	 * @param rotation The quaternion rotation of the GameObject.
	 */
	virtual void setRotation(const glm::quat &rotation) { mRotation = rotation; }

	/**
	 * Gets the scale of the GameObject.
	 * @return the scale vector of the GameObject.
	 */
	virtual glm::vec3 getScale() { return mScale; }

	/**
	 * Sets the scale of the GameObject.
	 * @param scale The scale vector of the GameObject.
	 */
	virtual void setScale(const glm::vec3 &scale) { mScale = scale; }

	/**
	 * Updates the camera transformation based upon input and mspf.
	 * @param movement The move struct that contains input for this frame.
	 * @param deltaMS The delta between the last and current frame.
	 */
	virtual void updateCamera(const Movement &movement, const F64 &deltaMS);

	/**
	 * Updates the movement of the GameObject based upon input and mspf.
	 * @param movement The move struct that contains input for this frame.
	 * @param deltaMS The delta between the last and current frame.
	*/
	virtual void updateMove(const Movement &movement, const F64 &deltaMS);

	/**
	 * Gets the camera position from the GameObject's perspective.
	 * @param OUT mat The camera matrix from the GameObject's perspective.
	 * @param OUT pos The position of the GameObject.
	 */
	virtual void getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos);

	/**
	 * An update callback that is called every physics tick.
	 * @param deltaMS The delta between physics ticks.
	 * @note Called at a fixed timestep.
	 */
	virtual void updateTick(const F64 &deltaMS) = 0;
};

#endif
