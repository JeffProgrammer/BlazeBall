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

	virtual bool isReflectable() { return true; }

	virtual void render(RenderInfo &info);
	virtual inline void updateTick(const F64 &delta) {};

	virtual void onAddToScene();

	/**
	* Initializes the fields specific to the GameObject class.
	*/
	static void initFields();
};

#endif /* skybox_h */
