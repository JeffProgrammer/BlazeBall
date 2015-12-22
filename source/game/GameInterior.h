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
#include "renderedObject.h"

#include <dif/objects/dif.h>
#include <vector>

class GameInterior : public RenderedObject {
protected:
	DIF::Interior mInterior;
	std::vector<Material*> mMaterialList;
	Texture *mNoiseTexture;

	PhysicsBody *mActor;
	
	struct RenderInfo {
		std::vector<U32> numMaterialTriangles;
		bool generated;
	};
	RenderInfo mRenderInfo;

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

	const DIF::Interior &getInterior() {return mInterior;}
	void setInterior(const DIF::Interior &interior) { mInterior = interior; }

	void gfxInit();
	void init();

	virtual bool isReflectable() { return true; }
	virtual void render(::RenderInfo &info);
	void drawMaterial(Material *material, ::RenderInfo &info, void *userInfo);

	virtual const glm::vec3 getPosition() const;
	virtual const glm::quat getRotation() const;

	virtual void setPosition(const glm::vec3 &pos);
	virtual void setRotation(const glm::quat &rot);

	virtual const glm::vec3 getScale() const;
	virtual void setScale(const glm::vec3 &scale);
	
	virtual inline void updateTick(const F64 &delta) {};
};

#endif
