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
			Vec3 point0 = Vec3(cosi * cosy, siny, sini * cosy);
			Vec3 point1 = Vec3(cosi * cosy1, siny1, sini * cosy1);

			glm::vec2 uv0 = glm::vec2((F32)i / (F32)segments2, (F32)y / (F32)slices2);
			glm::vec2 uv1 = glm::vec2((F32)i / (F32)segments2, (F32)(y + 1) / (F32)slices2);

			Vec3 tangent0 = glm::normalize(glm::cross(point0, Vec3(0, 0, 1)));
			Vec3 tangent1 = glm::normalize(glm::cross(point1, Vec3(0, 0, 1)));
			Vec3 bitangent0 = glm::normalize(glm::cross(point0, tangent0));
			Vec3 bitangent1 = glm::normalize(glm::cross(point1, tangent1));

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

void Sphere::calculateModelMatrix(const RenderInfo &info, Mat4 &modelMatrix) {
	RenderedObject::calculateModelMatrix(info, modelMatrix);

	modelMatrix = Mat4::scale(modelMatrix, Vec3(mRadius));
}

void Sphere::draw(Material *material, RenderInfo &info, void *userInfo) {
	loadModelMatrix(info, material->getShader());
	material->getShader()->setUniform("reflectivity", 0.2f);

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

void Sphere::applyTorque(const Vec3 &torque) {
	mActor->applyTorque(torque);
}

void Sphere::applyImpulse(const Vec3 &force, const Vec3 &origin) {
	mActor->applyImpulse(force, origin);
}

void Sphere::applyForce(const Vec3 &force, const Vec3 &origin) {
	mActor->applyForce(force, origin);
}

bool Sphere::getColliding() {
	return dynamic_cast<PhysicsSphere *>(mActor)->getColliding();
}

Vec3 Sphere::getCollisionNormal(Vec3 &toiVelocity) {
	return dynamic_cast<PhysicsSphere *>(mActor)->getCollisionNormal(toiVelocity);
}

Vec3 Sphere::getPosition() const {
	if (mActor)
		return mActor->getPosition();
	else
		return GameObject::getPosition();
}

Quat Sphere::getRotation() const {
	if (mActor)
		return mActor->getRotation();
	else
		return GameObject::getRotation();
}

Vec3 Sphere::getLinearVelocity() const {
	if (mActor)
		return mActor->getLinearVelocity();
	else
		return mLinearVelocity;
}

Vec3 Sphere::getAngularVelocity() const {
	if (mActor)
		return mActor->getAngularVelocity();
	else
		return mAngularVelocity;
}

Vec3 Sphere::getForce() const {
	if (mActor)
		return mActor->getForce();
	else
		return mForce;
}

Vec3 Sphere::getTorque() const {
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

void Sphere::setPosition(const Vec3 &pos) {
	if (mActor)
		mActor->setPosition(pos);
	else
		GameObject::setPosition(pos);
}

void Sphere::setRotation(const Quat &rot) {
	if (mActor)
		mActor->setRotation(rot);
	else
		GameObject::setRotation(rot);
}

void Sphere::setLinearVelocity(const Vec3 &vel) {
	if (mActor)
	    mActor->setLinearVelocity(vel);
	else
		mLinearVelocity = vel;
}

void Sphere::setAngularVelocity(const Vec3 &vel) {
	if (mActor)
	    mActor->setAngularVelocity(vel);
	else
		mAngularVelocity = vel;
}

void Sphere::setForce(const Vec3 &force) {
	if (mActor)
		mActor->setForce(force);
	else
		mForce = force;
}

void Sphere::setTorque(const Vec3 &torque) {
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
	//Apply the camera yaw to a matrix so our rolling is based on the camera direction
	Mat4 deltaMat = Mat4(1.0f);
	deltaMat = Mat4::rotate(deltaMat, -mCameraYaw, Vec3(0, 0, 1));

	//Convert the movement into a vector
	Vec3 move = Vec3();
	if (movement.forward) move.y ++;
	if (movement.backward) move.y --;
	if (movement.left) move.x --;
	if (movement.right) move.x ++;

	//Linear velocity relative to camera yaw (for capping)
	const Vec3 &linRel = Vec3(Mat4::translate(Mat4::inverse(deltaMat), mActor->getLinearVelocity())[3]);
	const Vec3 &angRel = Vec3::cross(Vec3(Mat4::translate(Mat4::inverse(deltaMat), mActor->getAngularVelocity())[3]), Vec3(0, 0, 1));
	Vec3 torque = move * AppliedAcceleration * F32(delta) * getRadius();

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
	Vec3 torqueRel = Vec3(Mat4::translate(deltaMat, torque)[3]);
	//Cross to convert 3d coordinates into torque
	applyTorque(Vec3::cross(Vec3(0, 0, 1), torqueRel));

	//If we are colliding with the ground, we have the chance to jump
	if (getColliding()) {
		//Impact velocity is stored when we collide so we can use it here
		Vec3 impactVelocity;
		//Get collision information
		Vec3 normal = getCollisionNormal(impactVelocity);
		Vec3 vel = mActor->getLinearVelocity();

		// dot against up vector to determine if we can jump
		// TODO: take gravities into account
		Vec3 up = Vec3(0.0f, 0.0f, 1.0f);
		if (movement.jump && Vec3::dot(up, normal) > 0.001f) {
			Vec3 currentVelocity = Vec3::project(vel, normal);

			Vec3 projVel = Vec3::project(vel, normal);

			if (projVel.length() < JumpImpulse) {
				Vec3 finalVelocity = vel - currentVelocity + (normal * JumpImpulse);
				setLinearVelocity(finalVelocity);
				IO::printf("Jump! Impact velocity: %f %f %f\n   final Velocity: %f %f %f\n    Projection velocity: %f %f %f\n    Dot: %f\n", vel.x, vel.y, vel.z, finalVelocity.x, finalVelocity.y, finalVelocity.z, projVel.x, projVel.y, projVel.z, Vec3::dot(up, normal));
			} else {
				IO::printf("No jump, projection velocity is %f %f %f\n", projVel.x, projVel.y, projVel.z);
			}
		}
	} else {
		Vec3 moveRel = Vec3(Mat4::translate(deltaMat, move)[3]);
		moveRel *= AirAcceleration * delta * getMass();

		//If we're not touching the ground, apply slight air movement.
		applyImpulse(moveRel, Vec3(0, 0, 0));
	}
	//Crappy damping
	if (movement.forward + movement.backward + movement.left + movement.right == 0 && getColliding()) {
		F32 damping = 1.f - LinearRollDamping;
		mActor->setAngularVelocity(mActor->getAngularVelocity() * damping);
	}
}

void Sphere::getCameraPosition(Mat4 &mat, Vec3 &pos) {
	//Reset the matrix
	Mat4 rotation(1.0f);

	//Rotate camera around the marble, also rotating the offset above
	rotation = Mat4::rotate(rotation, mCameraPitch, Vec3(1.0f, 0.0f, 0.0f));
	rotation = Mat4::rotate(rotation, mCameraYaw, Vec3(0.0f, 0.0f, 1.0f));

	//Reset
	mat = Mat4(1.0f);

	//Offset from marble; this is rotated by pitch and yaw
	mat = Mat4::translate(mat, Vec3(0.0f, 2.5f, 0.0f));

	//Rotate the matrix
	mat *= rotation;

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = Mat4::translate(mat, -getPosition());

	//Final position of the camera
	Vec3 rot = glm::mat3(Mat4::inverse(mat)) * Vec3(0, -2.5, 0);
	pos = getPosition() + rot;

	//Test camera for collisions
	PhysicsEngine::RaycastInfo info;
	info.from = getPosition();
	info.to = pos;

	mWorld->getPhysicsEngine()->raycast(info);

	//If we hit the ground, offset the camera so we can still see
	if (info.hit) {
		mat = rotation;
		mat = Mat4::translate(mat, -info.point);

		pos = info.point;
	}
}

void Sphere::updateTick(const F64 &delta) {
	//Temporary OOB solution for now
	if (getPosition().z < -40) {
		setPosition(Vec3(0, 0, 60));
		setLinearVelocity(Vec3(0, 0, 0));
		setAngularVelocity(Vec3(0, 0, 0));
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

	setPosition(stream.pop<Vec3>());
	setRotation(stream.pop<Quat>());
	setScale(stream.pop<Vec3>());

	setLinearVelocity(stream.pop<Vec3>());
	setAngularVelocity(stream.pop<Vec3>());

	setForce(stream.pop<Vec3>());
	setTorque(stream.pop<Vec3>());

	return true;
}

bool Sphere::readServerPacket(CharStream &stream) {
	if (!NetObject::readServerPacket(stream))
		return false;

	setPosition(stream.pop<Vec3>());
	setRotation(stream.pop<Quat>());
	setScale(stream.pop<Vec3>());

	setLinearVelocity(stream.pop<Vec3>());
	setAngularVelocity(stream.pop<Vec3>());

	setForce(stream.pop<Vec3>());
	setTorque(stream.pop<Vec3>());

	setRadius(stream.pop<F32>());
	setMass(stream.pop<F32>());

	return true;
}

bool Sphere::writeClientPacket(CharStream &stream) const {
	if (!NetObject::writeClientPacket(stream))
		return false;

	stream.push<Vec3>(getPosition());
	stream.push<Quat>(getRotation());
	stream.push<Vec3>(getScale());

	stream.push<Vec3>(getLinearVelocity());
	stream.push<Vec3>(getAngularVelocity());

	stream.push<Vec3>(getForce());
	stream.push<Vec3>(getTorque());

	return true;
}

bool Sphere::writeServerPacket(CharStream &stream) const {
	if (!NetObject::writeServerPacket(stream))
		return false;

	stream.push<Vec3>(getPosition());
	stream.push<Quat>(getRotation());
	stream.push<Vec3>(getScale());

	stream.push<Vec3>(getLinearVelocity());
	stream.push<Vec3>(getAngularVelocity());

	stream.push<Vec3>(getForce());
	stream.push<Vec3>(getTorque());

	stream.push<F32>(getRadius());
	stream.push<F32>(getMass());

	return true;
}

void Sphere::initFields() {
	// TODO
}

void Sphere::initScript(ScriptEngine *engine) {
	engine->addMethod(&Sphere::getRadius, "getRadius");
	engine->addMethod(&Sphere::setRadius, "setRadius");
}

// OLD JUMP CODE
//Make sure we're not trying to jump off a wall. Anything with a dot product > 0.1 is considered "not a wall"
//Vec3 normal = getCollisionNormal();
//if (movement.jump && glm::dot(normal, Vec3(0, 0, 1)) > 0.1) {
//Jump takes the average of the collision normal and the up vector to provide a mostly upwards
// jump but still taking the surface into account.
//Vec3 jumpNormal = (normal + Vec3(0, 0, 1)) / 2.f;
//Vec3 vel = Vec3(0, 0, 0);//mActor->getLinearVelocity();

//Vec3 proj = jumpNormal * (glm::dot(vel, jumpNormal) * glm::dot(jumpNormal, jumpNormal));
//Vec3 imp = jumpNormal * (JumpImpulse - glm::length(proj));
//IO::printf("jump normal: %f %f %f\nvel: %f %f %f\nproj: %f %f %f\nimp: %f %f %f",
// jumpNormal.x, jumpNormal.y, jumpNormal.z,
//vel.x, vel.y, vel.z,
// proj.x, proj.y, proj.z,
//imp.x, imp.y, imp.z
// );

//if (glm::length(proj) < JumpImpulse) {
//applyImpulse(imp, Vec3(0, 0, 0));
//}