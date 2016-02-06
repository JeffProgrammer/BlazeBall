//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_SPHERE_H_
#define _GAME_SPHERE_H_

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
#include "render/texture/cubeMapFramebufferTexture.h"

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
	Movement mMovement;

	glm::vec3 mLinearVelocity;
	glm::vec3 mAngularVelocity;
	glm::vec3 mForce;
	glm::vec3 mTorque;
	F32 mMass;
	F32 mRadius;

	CubeMapFramebufferTexture *mCubemap;

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

	virtual void calculateModelMatrix(const RenderInfo &info, glm::mat4 &modelMatrix) override;
	virtual void render(RenderInfo &info) override;
	void draw(Material *material, RenderInfo &info, void *userInfo);

	virtual glm::vec3 getPosition() const override;
	virtual glm::quat getRotation() const override;
	glm::vec3 getLinearVelocity() const;
	glm::vec3 getAngularVelocity() const;
	glm::vec3 getForce() const;
	glm::vec3 getTorque() const;
	F32 getRadius() const;
	F32 getMass() const;

	virtual void setPosition(const glm::vec3 &pos) override;
	virtual void setRotation(const glm::quat &rot) override;
	void setLinearVelocity(const glm::vec3 &vel);
	void setAngularVelocity(const glm::vec3 &vel);
	void setForce(const glm::vec3 &force);
	void setTorque(const glm::vec3 &torque);
	void setRadius(const F32 &radius);
	void setMass(const F32 &mass);
	void setActivationState(bool active);

	void setMaterial(Material *material) {
		this->mMaterial = material;
	}

	void applyTorque(const glm::vec3 &torque);
	void applyImpulse(const glm::vec3 &force, const glm::vec3 &origin);
	void applyForce(const glm::vec3 &force, const glm::vec3 &origin);

	bool getColliding();
	glm::vec3 getCollisionNormal(glm::vec3 &toiVelocity);
    
	virtual void updateCamera(const Movement &movement, const F64 &delta) override;
	virtual void updateMove(const Movement &movement, const F64 &delta) override;
	virtual void getCameraPosition(glm::mat4 &mat, glm::vec3 &pos) override;

	virtual void updateTick(const F64 &delta) override;
	virtual void onAddToScene() override;

	virtual bool readClientPacket(CharStream &stream) override;
	virtual bool readServerPacket(CharStream &stream) override;
	virtual bool writeClientPacket(CharStream &stream) const override;
	virtual bool writeServerPacket(CharStream &stream) const override;

	static void initFields();
};

#endif // _GAME_SPHERE_H_