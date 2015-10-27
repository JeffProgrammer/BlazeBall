//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef gameInterior_h
#define gameInterior_h

#include "base/io.h"
#include "base/types.h"
#include "render/material.h"
#include "physics/physicsEngine.h"
#include "game/gameObject.h"
#include "base/ray.h"
#include "renderedObject.h"

#include <dif/objects/dif.h>
#include <vector>

class GameInterior : public RenderedObject {
private:
	DIF::Interior mInterior;
	std::vector<Material*> mMaterialList;
	Texture *mNoiseTexture;

	PhysicsBody *mActor;
	
	struct RenderInfo {
		std::vector<U32> numMaterialTriangles;
		bool generated;
	};
	RenderInfo renderInfo;

	struct RenderUserData {
		U32 numTriangles;
		U32 start;
	};
	
	GLuint mVbo;

public:
	GameInterior();
	GameInterior(DIF::Interior interior);
	virtual ~GameInterior();
	
	void generateMaterials(std::string directory);
	void generateMesh();
	void exportObj(FILE *file);
	U32 rayCast(RayF ray);

	const DIF::Interior &getInterior() {return mInterior;}
	void setInterior(const DIF::Interior &interior) { mInterior = interior; }

	void gfxInit();
	void init();

	virtual bool isReflectable() { return true; }
	virtual void render(::RenderInfo &info);
	void drawMaterial(Material *material, ::RenderInfo &info, void *userInfo);

	virtual glm::vec3 getPosition();
	virtual glm::quat getRotation();

	virtual void setPosition(const glm::vec3 &pos);
	virtual void setRotation(const glm::quat &rot);

	virtual glm::vec3 getScale();
	virtual void setScale(const glm::vec3 &scale);
	
	virtual inline void updateTick(const F64 &deltaMS) {};
};

#endif
