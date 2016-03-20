//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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