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
	typedef RenderedObject Parent;
protected:
	std::string mInteriorFile;

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

	DECLARE_SCRIPTOBJECT(GameInterior);
public:
	GameInterior(World *world);
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

	virtual Vec3 getPosition() const override;
	virtual Quat getRotation() const override;
	virtual Vec3 getScale() const override;

	virtual void setPosition(const Vec3 &pos) override;
	virtual void setRotation(const Quat &rot) override;
	virtual void setScale(const Vec3 &scale) override;
	
	virtual inline void updateTick(const F64 &delta) override {};

	virtual void onAddToScene() override;

	virtual bool read(CharStream &stream);
	virtual bool write(CharStream &stream) const;

	static void initFields();

	/**
	 * Initializes the scripting API for the respective sript engine.
	 * @param engine The script engine to initialize to.
	 */
	static void initScript(ScriptEngine *engine);
};

#endif
