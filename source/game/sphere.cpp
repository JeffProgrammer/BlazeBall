//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/sphere.h"
#include "base/math.h"
#include "base/io.h"
#include "physics/physicsSphere.h"
#include "render/renderWorld.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>

IMPLEMENT_SCRIPTOBJECT(Sphere, RenderedObject);

GLuint gSphereVBO = 0;

Sphere::Sphere() : RenderedObject(), mActor(nullptr) {
	mGenerated = false;

	mCameraYaw = 0.0f;
	mCameraPitch = 0.0f;
	}

Sphere::~Sphere() {
	// todo: remove body from physics engine
}

void Sphere::generate() {
	if (gSphereVBO)
		return;

	F32 step = (glm::pi<F32>() * 2.0f / segments);

	S32 segments2 = segments / 2;
	S32 slices2 = slices / 2;

	S32 size = segments * slices * 2;
	Vertex *points = new Vertex[size];

	U32 point = 0;

	for (S32 y = -slices2; y < slices2; y ++) {
		float cosy = cos(y * step);
		float cosy1 = cos((y + 1) * step);
		float siny = sin(y * step);
		float siny1 = sin((y + 1) * step);

		for (S32 i = -segments2; i < segments2; i ++) {
			float cosi = cos(i * step);
			float sini = sin(i * step);

			//Math not invented by me
			glm::vec3 point0 = glm::vec3(cosi * cosy, siny, sini * cosy);
			glm::vec3 point1 = glm::vec3(cosi * cosy1, siny1, sini * cosy1);

			glm::vec2 uv0 = glm::vec2((F32)i / (F32)segments2, (F32)y / (F32)slices2);
			glm::vec2 uv1 = glm::vec2((F32)i / (F32)segments2, (F32)(y + 1) / (F32)slices2);

			glm::vec3 tangent0 = glm::normalize(glm::cross(point0, glm::vec3(0, 0, 1)));
			glm::vec3 tangent1 = glm::normalize(glm::cross(point1, glm::vec3(0, 0, 1)));
			glm::vec3 bitangent0 = glm::normalize(glm::cross(point0, tangent0));
			glm::vec3 bitangent1 = glm::normalize(glm::cross(point1, tangent1));

			points[point].point = point0;
			points[point].uv = uv0;
			points[point].normal = point0;
			points[point].tangent = tangent0;
			points[point].bitangent = bitangent0;
			point ++;
			points[point].point = point1;
			points[point].uv = uv1;
			points[point].normal = point1;
			points[point].tangent = tangent1;
			points[point].bitangent = bitangent1;
			point ++;
		}
	}

	//Generate a buffer
	glGenBuffers(1, &gSphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gSphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * point, &points[0], GL_STATIC_DRAW);
}

void Sphere::calculateModelMatrix(const RenderInfo &info, glm::mat4 &modelMatrix) {
	RenderedObject::calculateModelMatrix(info, modelMatrix);

	modelMatrix = glm::scale(modelMatrix, glm::vec3(mRadius));
}

void Sphere::draw(Material *material, RenderInfo &info, void *userInfo) {
	loadModelMatrix(info, material->getShader());
	material->getShader()->setUniform("reflectivity", 0.3f);

	glBindBuffer(GL_ARRAY_BUFFER, gSphereVBO);
	material->getShader()->enableAttributes();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, segments * slices * 2);

	material->getShader()->disableAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sphere::render(RenderInfo &info) {
	if (!mGenerated) {
		generate();
		mGenerated = true;
	}

	if (!mMaterial)
		return;

	info.addRenderMethod(mMaterial, RenderInfo::RenderMethod::from_method<Sphere, &Sphere::draw>(this));
}

void Sphere::applyTorque(const glm::vec3 &torque) {
	mActor->applyTorque(torque);
}

void Sphere::applyImpulse(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyImpulse(force, origin);
}

void Sphere::applyForce(const glm::vec3 &force, const glm::vec3 &origin) {
	mActor->applyForce(force, origin);
}

bool Sphere::getColliding() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getColliding();
}

glm::vec3 Sphere::getCollisionNormal(glm::vec3 &toiVelocity) {
	return dynamic_cast<PhysicsSphere *>(mActor)->getCollisionNormal(toiVelocity);
}

glm::vec3 Sphere::getPosition() const {
	if (mActor)
		return mActor->getPosition();
	else
		return GameObject::getPosition();
}

glm::quat Sphere::getRotation() const {
	if (mActor)
		return mActor->getRotation();
	else
		return GameObject::getRotation();
}

glm::vec3 Sphere::getLinearVelocity() const {
	if (mActor)
		return mActor->getLinearVelocity();
	else
		return mLinearVelocity;
}

glm::vec3 Sphere::getAngularVelocity() const {
	if (mActor)
		return mActor->getAngularVelocity();
	else
		return mAngularVelocity;
}

glm::vec3 Sphere::getForce() const {
	if (mActor)
		return mActor->getForce();
	else
		return mForce;
}

glm::vec3 Sphere::getTorque() const {
	if (mActor)
		return mActor->getTorque();
	else
		return mTorque;
}

F32 Sphere::getRadius() const {
	if (mActor)
		return dynamic_cast<PhysicsSphere *>(mActor)->getRadius();
	else
		return mRadius;
}

F32 Sphere::getMass() const {
	if (mActor)
		return mActor->getMass();
	else
		return mMass;
}

void Sphere::setPosition(const glm::vec3 &pos) {
	if (mActor)
		mActor->setPosition(pos);
	else
		GameObject::setPosition(pos);
}

void Sphere::setRotation(const glm::quat &rot) {
	if (mActor)
		mActor->setRotation(rot);
	else
		GameObject::setRotation(rot);
}

void Sphere::setLinearVelocity(const glm::vec3 &vel) {
	if (mActor)
	    mActor->setLinearVelocity(vel);
	else
		mLinearVelocity = vel;
}

void Sphere::setAngularVelocity(const glm::vec3 &vel) {
	if (mActor)
	    mActor->setAngularVelocity(vel);
	else
		mAngularVelocity = vel;
}

void Sphere::setForce(const glm::vec3 &force) {
	if (mActor)
		mActor->setForce(force);
	else
		mForce = force;
}

void Sphere::setTorque(const glm::vec3 &torque) {
	if (mActor)
		mActor->setTorque(torque);
	else
		mTorque = torque;
}

void Sphere::setRadius(const F32 &radius) {
	if (mActor)
		dynamic_cast<PhysicsSphere *>(mActor)->setRadius(radius);
	else
		mRadius = radius;
}

void Sphere::setMass(const F32 &mass) {
	if (mActor)
		mActor->setMass(mass);
	else
		mMass = mass;
}

void Sphere::setActivationState(bool active) {
	if (mActor)
		mActor->setActivationState(active);
}

void Sphere::updateCamera(const Movement &movement, const F64 &delta) {
	if (movement.pitchUp) mCameraPitch -= keyCameraSpeed;
	if (movement.pitchDown) mCameraPitch += keyCameraSpeed;
	if (movement.yawLeft) mCameraYaw -= keyCameraSpeed;
	if (movement.yawRight) mCameraYaw += keyCameraSpeed;

	mCameraPitch += movement.pitch * cameraSpeed;
	mCameraYaw += movement.yaw * cameraSpeed;
}
void Sphere::updateMove(const Movement &movement, const F64 &delta) {
	mMovement = movement;
}

void Sphere::getCameraPosition(glm::mat4 &mat, glm::vec3 &pos) {
	//Reset the matrix
	glm::mat4 rotation(1.0f);

	//Rotate camera around the marble, also rotating the offset above
	rotation = glm::rotate(rotation, mCameraPitch, glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, mCameraYaw, glm::vec3(0.0f, 0.0f, 1.0f));

	//Reset
	mat = glm::mat4(1.0f);

	//Offset from marble; this is rotated by pitch and yaw
	mat = glm::translate(mat, glm::vec3(0.0f, 2.5f, 0.0f));

	//Rotate the matrix
	mat *= rotation;

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = glm::translate(mat, -getPosition());

	//Final position of the camera
	glm::vec3 rot = glm::mat3(glm::inverse(mat)) * glm::vec3(0, -2.5, 0);
	pos = getPosition() + rot;

	//Test camera for collisions
	PhysicsEngine::RaycastInfo info;
	info.from = getPosition();
	info.to = pos;

	mWorld->getPhysicsEngine()->raycast(info);

	//If we hit the ground, offset the camera so we can still see
	if (info.hit) {
		mat = rotation;
		mat = glm::translate(mat, -info.point);

		pos = info.point;
	}
}

void Sphere::updateTick(const F64 &delta) {
	//Apply the camera yaw to a matrix so our rolling is based on the camera direction
	glm::mat4x4 deltaMat = glm::mat4x4(1);
	deltaMat = glm::rotate(deltaMat, -mCameraYaw, glm::vec3(0, 0, 1));

	//Convert the movement into a vector
	glm::vec3 move = glm::vec3();
	if (mMovement.forward) move.y ++;
	if (mMovement.backward) move.y --;
	if (mMovement.left) move.x --;
	if (mMovement.right) move.x ++;

	//Linear velocity relative to camera yaw (for capping)
	glm::vec3 linRel = glm::vec3(glm::translate(glm::inverse(deltaMat), mActor->getLinearVelocity())[3]);
	glm::vec3 angRel = glm::cross(glm::vec3(glm::translate(glm::inverse(deltaMat), mActor->getAngularVelocity())[3]), glm::vec3(0, 0, 1));
	glm::vec3 torque = move * AppliedAcceleration * F32(delta) * getRadius();

	if (getColliding()) {
		//Don't let us go faster than the max velocity in any direction.
		if (torque.x > 0 && torque.x + linRel.x >  MaxRollVelocity) torque.x = glm::max(0.f,  MaxRollVelocity - linRel.x);
		if (torque.y > 0 && torque.y + linRel.y >  MaxRollVelocity) torque.y = glm::max(0.f,  MaxRollVelocity - linRel.y);
		//Same for backwards
		if (torque.x < 0 && torque.x + linRel.x < -MaxRollVelocity) torque.x = glm::min(0.f, -MaxRollVelocity - linRel.x);
		if (torque.y < 0 && torque.y + linRel.y < -MaxRollVelocity) torque.y = glm::min(0.f, -MaxRollVelocity - linRel.y);
	} else {
		//Don't let us roll faster than the max air roll velocity
		if (torque.x > 0 && torque.x + angRel.x >  MaxAirSpinVelocity) torque.x = glm::max(0.0f,  MaxAirSpinVelocity - angRel.x);
		if (torque.y > 0 && torque.y + angRel.y >  MaxAirSpinVelocity) torque.y = glm::max(0.0f,  MaxAirSpinVelocity - angRel.y);
		if (torque.x < 0 && torque.x + angRel.x < -MaxAirSpinVelocity) torque.x = glm::min(0.0f, -MaxAirSpinVelocity - angRel.x);
		if (torque.y < 0 && torque.y + angRel.y < -MaxAirSpinVelocity) torque.y = glm::min(0.0f, -MaxAirSpinVelocity - angRel.y);
	}

	//	IO::printf("LR: %f %f\n", linRel.x, linRel.y);
	//	IO::printf("AR: %f %f\n", angRel.x, angRel.y);
	//	IO::printf("T:  %f %f\n", torque.x, torque.y);

	//Torque is based on the movement and yaw
	glm::vec3 torqueRel = glm::vec3(glm::translate(deltaMat, torque)[3]) * getMass();
	//Cross to convert 3d coordinates into torque
	applyTorque(glm::cross(glm::vec3(0, 0, 1), torqueRel));

	//If we are colliding with the ground, we have the chance to jump
	if (getColliding()) {
		//Impact velocity is stored when we collide so we can use it here
		glm::vec3 impactVelocity;
		//Get collision information
		glm::vec3 normal = getCollisionNormal(impactVelocity);
		glm::vec3 vel = mActor->getLinearVelocity();

		// dot against up vector to determine if we can jump
		// TODO: take gravities into account
		glm::vec3 up = glm::vec3(0, 0, 1);
		if (mMovement.jump && glm::dot(up, normal) > 0.001f) {
			glm::vec3 currentVelocity = glm::proj(vel, normal);

			glm::vec3 projVel = glm::proj(vel, normal);

			if (glm::length(projVel) < JumpImpulse) {
				glm::vec3 finalVelocity = vel - currentVelocity + (normal * JumpImpulse);
				setLinearVelocity(finalVelocity);
				IO::printf("Jump! Impact velocity: %f %f %f\n   final Velocity: %f %f %f\n    Projection velocity: %f %f %f\n    Dot: %f\n", vel.x, vel.y, vel.z, finalVelocity.x, finalVelocity.y, finalVelocity.z, projVel.x, projVel.y, projVel.z, glm::dot(up, normal));
			} else {
				IO::printf("No jump, projection velocity is %f %f %f\n", projVel.x, projVel.y, projVel.z);
			}
		}
	} else {
		glm::vec3 moveRel = glm::vec3(glm::translate(deltaMat, move)[3]);
		moveRel *= AirAcceleration * delta * getMass();

		//If we're not touching the ground, apply slight air movement.
		applyImpulse(moveRel, glm::vec3(0, 0, 0));
	}
	//Crappy damping
	if (mMovement.forward + mMovement.backward + mMovement.left + mMovement.right == 0 && getColliding()) {
		F32 damping = 1.f - LinearRollDamping;
		mActor->setAngularVelocity(mActor->getAngularVelocity() * damping);
	}

	//Temporary OOB solution for now
	if (getPosition().z < -40) {
		setPosition(glm::vec3(0, 0, 60));
		setLinearVelocity(glm::vec3(0, 0, 0));
		setAngularVelocity(glm::vec3(0, 0, 0));
	}
}

void Sphere::onAddToScene() {
	mActor = mWorld->getPhysicsEngine()->createSphere(mRadius);
	mActor->setPosition(mPosition);
	mActor->setMass(mMass);
	mActor->setWorld(mWorld);
	mWorld->getPhysicsEngine()->addBody(mActor);

	mActor->setActivationState(false);

	RenderWorld *renderer = dynamic_cast<RenderWorld *>(mWorld);
	if (renderer) {
		Material *material = new Material("marble.skin");
		material->setTexture(renderer->mMarbleCubemap, GL_TEXTURE3);
		material->setShader(Shader::getShaderByName("Sphere"));
		setMaterial(material);
	}
}

bool Sphere::readClientPacket(CharStream &stream) {
	if (!NetObject::readClientPacket(stream))
		return false;

	setPosition(stream.pop<glm::vec3>());
	setRotation(stream.pop<glm::quat>());
	setScale(stream.pop<glm::vec3>());

	setLinearVelocity(stream.pop<glm::vec3>());
	setAngularVelocity(stream.pop<glm::vec3>());

	setForce(stream.pop<glm::vec3>());
	setTorque(stream.pop<glm::vec3>());

	mCameraYaw = stream.pop<F32>();
	mCameraPitch = stream.pop<F32>();

	mMovement.forward = stream.pop<bool>();
	mMovement.backward = stream.pop<bool>();
	mMovement.left = stream.pop<bool>();
	mMovement.right = stream.pop<bool>();

	return true;
}

bool Sphere::readServerPacket(CharStream &stream) {
	if (!NetObject::readServerPacket(stream))
		return false;

	setPosition(stream.pop<glm::vec3>());
	setRotation(stream.pop<glm::quat>());
	setScale(stream.pop<glm::vec3>());

	setLinearVelocity(stream.pop<glm::vec3>());
	setAngularVelocity(stream.pop<glm::vec3>());

	setForce(stream.pop<glm::vec3>());
	setTorque(stream.pop<glm::vec3>());

	setRadius(stream.pop<F32>());
	setMass(stream.pop<F32>());

	mCameraYaw = stream.pop<F32>();
	mCameraPitch = stream.pop<F32>();

	mMovement.forward = stream.pop<bool>();
	mMovement.backward = stream.pop<bool>();
	mMovement.left = stream.pop<bool>();
	mMovement.right = stream.pop<bool>();

	return true;
}

bool Sphere::writeClientPacket(CharStream &stream) const {
	if (!NetObject::writeClientPacket(stream))
		return false;

	stream.push<glm::vec3>(getPosition());
	stream.push<glm::quat>(getRotation());
	stream.push<glm::vec3>(getScale());

	stream.push<glm::vec3>(getLinearVelocity());
	stream.push<glm::vec3>(getAngularVelocity());

	stream.push<glm::vec3>(getForce());
	stream.push<glm::vec3>(getTorque());

	stream.push<F32>(mCameraYaw);
	stream.push<F32>(mCameraPitch);

	stream.push<bool>(mMovement.forward);
	stream.push<bool>(mMovement.backward);
	stream.push<bool>(mMovement.left);
	stream.push<bool>(mMovement.right);

	return true;
}

bool Sphere::writeServerPacket(CharStream &stream) const {
	if (!NetObject::writeServerPacket(stream))
		return false;

	stream.push<glm::vec3>(getPosition());
	stream.push<glm::quat>(getRotation());
	stream.push<glm::vec3>(getScale());

	stream.push<glm::vec3>(getLinearVelocity());
	stream.push<glm::vec3>(getAngularVelocity());

	stream.push<glm::vec3>(getForce());
	stream.push<glm::vec3>(getTorque());

	stream.push<F32>(getRadius());
	stream.push<F32>(getMass());

	stream.push<F32>(mCameraYaw);
	stream.push<F32>(mCameraPitch);

	stream.push<bool>(mMovement.forward);
	stream.push<bool>(mMovement.backward);
	stream.push<bool>(mMovement.left);
	stream.push<bool>(mMovement.right);

	return true;
}

void Sphere::initScript(ScriptEngine *engine) {
	engine->addClass<Sphere, RenderedObject>("Sphere");
	engine->addMethod(&Sphere::getRadius, "getRadius");
	engine->addMethod(&Sphere::setRadius, "setRadius");
}

// OLD JUMP CODE
//Make sure we're not trying to jump off a wall. Anything with a dot product > 0.1 is considered "not a wall"
//glm::vec3 normal = getCollisionNormal();
//if (movement.jump && glm::dot(normal, glm::vec3(0, 0, 1)) > 0.1) {
//Jump takes the average of the collision normal and the up vector to provide a mostly upwards
// jump but still taking the surface into account.
//glm::vec3 jumpNormal = (normal + glm::vec3(0, 0, 1)) / 2.f;
//glm::vec3 vel = glm::vec3(0, 0, 0);//mActor->getLinearVelocity();

//glm::vec3 proj = jumpNormal * (glm::dot(vel, jumpNormal) * glm::dot(jumpNormal, jumpNormal));
//glm::vec3 imp = jumpNormal * (JumpImpulse - glm::length(proj));
//IO::printf("jump normal: %f %f %f\nvel: %f %f %f\nproj: %f %f %f\nimp: %f %f %f",
// jumpNormal.x, jumpNormal.y, jumpNormal.z,
//vel.x, vel.y, vel.z,
// proj.x, proj.y, proj.z,
//imp.x, imp.y, imp.z
// );

//if (glm::length(proj) < JumpImpulse) {
//applyImpulse(imp, glm::vec3(0, 0, 0));
//}