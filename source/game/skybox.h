//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef skybox_h
#define skybox_h

#include "gameObject.h"
#include "render/material.h"
#include "renderedObject.h"

class Skybox : public RenderedObject {
protected:
	GLuint mBuffer;
	bool mGenerated;

	Material *mMaterial;

	DECLARE_SCRIPTOBJECT(Skybox);
public:
	Skybox() : Skybox(nullptr) {}
	Skybox(Material *material);
	virtual ~Skybox();

	void generate();

	void setMaterial(Material *material) {
		mMaterial = material;
	}
	Material *getMaterial() {
		return mMaterial;
	}

	virtual bool isReflectable() { return true; }

	virtual void render(RenderInfo &info);
	virtual inline void updateTick(const F64 &delta) {};

	/**
	* Initializes the fields specific to the GameObject class.
	*/
	static void initFields();
};

#endif /* skybox_h */
