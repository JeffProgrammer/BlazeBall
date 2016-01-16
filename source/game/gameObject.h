//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef gameObject_h
#define gameObject_h

#include <glm/matrix.hpp>

#include "base/types.h"
#include "base/math.h"
#include "game/movement.h"
#include "render/renderInfo.h"
#include "network/netObject.h"
#include "scriptEngine/concreteClassRep.h"
#include "game/world.h"
#include "platform/platformGL.h"

/**
 * An class that forms the foundation for all world objects within the
 * game. This class is merely a shell for all subclasses of 3-dimentional
 * classes.
 */
class GameObject : public NetObject {
protected:
	/**
	 * The position of the GameObject.
	 */
	Vec3 mPosition;

	/**
	 * The rotation of the GameObject.
	 */
	Quat mRotation;

	/**
	 * The scale of the GameObject.
	 */
	Vec3 mScale;

	/**
	 * The class rep representing the GameObject abstract class
	 */
	DECLARE_SCRIPTOBJECT(GameObject);
	
public:
	GameObject() : mPosition(Vec3(0.0f)), mRotation(Quat()), mScale(Vec3(1.0f)) {};
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
	virtual Vec3 getPosition() const { return mPosition; }

	/**
	 * Sets the position of the GameObject.
	 * @param position The position to set the GameObject.
	 */
	virtual void setPosition(const Vec3 &position) { mPosition = position; }

	/**
	 * Gets the rotation of the GameObject as a quaternion.
	 * @return the rotation quaternion of the GameObject.
	 */
	virtual Quat getRotation() const { return mRotation; }

	/**
	 * Sets the rotation of the GameObject as a quaternion.
	 * @param rotation The quaternion rotation of the GameObject.
	 */
	virtual void setRotation(const Quat &rotation) { mRotation = rotation; }

	/**
	 * Gets the scale of the GameObject.
	 * @return the scale vector of the GameObject.
	 */
	virtual Vec3 getScale() const { return mScale; }

	/**
	 * Sets the scale of the GameObject.
	 * @param scale The scale vector of the GameObject.
	 */
	virtual void setScale(const Vec3 &scale) { mScale = scale; }

	/**
	 * Updates the camera.
	 * @param movement The move struct that contains input for this frame.
	 * @param delta The delta, in seconds, between the last and current frame.
	 */
	virtual void updateCamera(const Movement &movement, const F64 &delta);

	/**
	 * Updates the movement of the GameObject based upon input and mspf.
	 * @param movement The move struct that contains input for this frame.
	 * @param delta The delta, in seconds between the last and current frame.
	*/
	virtual void updateMove(const Movement &movement, const F64 &delta);

	/**
	 * Gets the camera position from the GameObject's perspective.
	 * @param OUT mat The camera matrix from the GameObject's perspective.
	 * @param OUT pos The position of the GameObject.
	 */
	virtual void getCameraPosition(Mat4 &mat, Vec3 &pos);

	/**
	 * An update callback that is called every physics tick.
	 * @param delta The delta, in seconds, between physics ticks.
	 * @note Called at a fixed timestep.
	 */
	virtual void updateTick(const F64 &delta) {}

	/**
	 * A callback that is called when the object is added to the scene.
	 */
	virtual void onAddToScene() {}

	virtual bool read(CharStream &stream);
	virtual bool write(CharStream &stream) const;

	/**
	 * Initializes the fields specific to the GameObject class.
	 */
	static void initFields();

	/**
	 * Initializes the scripting API for the respective sript engine.
	 * @param engine The script engine to initialize to.
	 */
	static void initScript(ScriptEngine *engine);
};

#endif
