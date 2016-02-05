//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_SKYBOX_H_
#define _GAME_SKYBOX_H_

#include "gameObject.h"
#include "render/material.h"
#include "renderedObject.h"

class Skybox : public RenderedObject {
	typedef RenderedObject Parent;
protected:
	GLuint mBuffer;
	bool mGenerated;

	/*
	 * The 6 faces of the cubemap
	 */
	std::string mFace0;
	std::string mFace1;
	std::string mFace2;
	std::string mFace3;
	std::string mFace4;
	std::string mFace5;

	Material *mMaterial;

	DECLARE_SCRIPTOBJECT(Skybox);
public:
	Skybox();
	virtual ~Skybox();

	void generate();

	void setMaterial(Material *material) {
		mMaterial = material;
	}
	Material *getMaterial() {
		return mMaterial;
	}

	virtual bool isReflectable() override { return true; }

	virtual void render(RenderInfo &info) override;

	virtual void onAddToScene() override;

	virtual bool read(CharStream &stream) override;
	virtual bool write(CharStream &stream) const override;

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

#endif // _GAME_SKYBOX_H_
