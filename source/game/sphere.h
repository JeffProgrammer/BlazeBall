//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef sphere_h
#define sphere_h

#include <stdio.h>
#include <vector>
#include <glm/matrix.hpp>
#include "platform/platformGL.h"
#include "gameObject.h"
#include "base/types.h"
#include "physics/physicsBody.h"
#include "render/material.h"
#include "game/movement.h"
#include "renderedObject.h"

class Sphere : public RenderedObject {
	const F32 AppliedAcceleration = 750.f; //Maximum, lowers when reaching max roll velocity
	const F32 MaxRollVelocity     = 15.f; //In one direction (diagonal-supported)
	const F32 MaxAirSpinVelocity  = 50.f; //Same as above but for angular, only when falling
	const F32 JumpImpulse         = 7.5f; //Per jump, magnitude of the vector
	const F32 AirAcceleration     = 5.f; //Applied every tick in the air
	const F32 LinearRollDamping   = 0.025f; // 97.5% of original

protected:
	PhysicsBody *mActor;
	F32 mMaxAngVel;
	Material *mMaterial;

	Vec3 mLinearVelocity;
	Vec3 mAngularVelocity;
	Vec3 mForce;
	Vec3 mTorque;
	F32 mMass;
	F32 mRadius;

	bool mGenerated;

	F32 mCameraYaw;
	F32 mCameraPitch;

	void generate();

	const U32 segments = 36;
	const U32 slices = 18;

	const F32 cameraSpeed = 0.005f;
	const F32 keyCameraSpeed = 0.05f;

	DECLARE_SCRIPTOBJECT(Sphere);

public:
	Sphere();
	virtual ~Sphere();

	virtual void calculateModelMatrix(const RenderInfo &info, Mat4 &modelMatrix) override;
	virtual void render(RenderInfo &info) override;
	void draw(Material *material, RenderInfo &info, void *userInfo);

	virtual Vec3 getPosition() const override;
	virtual Quat getRotation() const override;
	Vec3 getLinearVelocity() const;
	Vec3 getAngularVelocity() const;
	Vec3 getForce() const;
	Vec3 getTorque() const;
	F32 getRadius() const;
	F32 getMass() const;

	virtual void setPosition(const Vec3 &pos) override;
	virtual void setRotation(const Quat &rot) override;
	void setLinearVelocity(const Vec3 &vel);
	void setAngularVelocity(const Vec3 &vel);
	void setForce(const Vec3 &force);
	void setTorque(const Vec3 &torque);
	void setRadius(const F32 &radius);
	void setMass(const F32 &mass);
	void setActivationState(bool active);

	void setMaterial(Material *material) {
		this->mMaterial = material;
	}

	void applyTorque(const Vec3 &torque);
	void applyImpulse(const Vec3 &force, const Vec3 &origin);
	void applyForce(const Vec3 &force, const Vec3 &origin);

	bool getColliding();
	Vec3 getCollisionNormal(Vec3 &toiVelocity);
    
	virtual void updateCamera(const Movement &movement, const F64 &delta) override;
	virtual void updateMove(const Movement &movement, const F64 &delta) override;
	virtual void getCameraPosition(Mat4 &mat, Vec3 &pos) override;

	virtual void updateTick(const F64 &delta) override;
	virtual void onAddToScene() override;

	virtual bool readClientPacket(CharStream &stream) override;
	virtual bool readServerPacket(CharStream &stream) override;
	virtual bool writeClientPacket(CharStream &stream) const override;
	virtual bool writeServerPacket(CharStream &stream) const override;

	/**
	 * Initializes the scripting API for the respective sript engine.
	 * @param engine The script engine to initialize to.
	 */
	static void initScript(ScriptEngine *engine);
};

#endif