//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_GAMEINTERIOR_H_
#define _GAME_GAMEINTERIOR_H_

#include "base/io.h"
#include "base/types.h"
#include "render/material.h"
#include "physics/physicsEngine.h"
#include "physics/physicsInterior.h"
#include "game/gameObject.h"
#include "renderedObject.h"

#include <dif/objects/dif.h>
#include <vector>

class GameInterior : public RenderedObject {
	typedef RenderedObject Parent;
protected:
	std::string mInteriorFile;

	DIF::Interior mInterior;
	std::vector<Material*> mMaterialList;
	Texture *mNoiseTexture;

	PhysicsInterior *mActor;
	
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

	DECLARE_SCRIPTOBJECT(GameInterior);
public:
	GameInterior();
	virtual ~GameInterior();
	
	void generateMaterials(std::string directory);
	void generateMesh();
	void exportObj(FILE *file);

	const DIF::Interior &getInterior() {return mInterior;}
	void setInterior(const DIF::Interior &interior) { mInterior = interior; }

	void gfxInit();
	void init();

	virtual bool isReflectable() override { return true; }
	virtual void render(::RenderInfo &info) override;
	void drawMaterial(Material *material, ::RenderInfo &info, void *userInfo);

	virtual glm::vec3 getPosition() const override;
	virtual glm::quat getRotation() const override;
	virtual glm::vec3 getScale() const override;

	virtual void setPosition(const glm::vec3 &pos) override;
	virtual void setRotation(const glm::quat &rot) override;
	virtual void setScale(const glm::vec3 &scale) override;
	
	virtual inline void updateTick(const F64 &delta) override {};

	virtual void onAddToScene() override;

	virtual bool read(CharStream &stream) override;
	virtual bool write(CharStream &stream) const override;

	static void initFields();
};

#endif // _GAME_GAMEINTERIOR_H_