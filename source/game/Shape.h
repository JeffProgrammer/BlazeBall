//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_SHAPE_H_
#define _GAME_SHAPE_H_

#include "game/gameObject.h"
#include "render/renderInfo.h"
#include "renderedObject.h"

class Shape : public RenderedObject {
private:
	typedef RenderedObject Parent;

protected:
	std::string mShapeFile;

public:
	Shape(const std::string &shapeFile);
	virtual ~Shape();
	virtual void updateTick(const F64 &delta);
	virtual void render(RenderInfo &info);

	void loadShape();

	const std::string& getShapeFile() const {
		return mShapeFile;
	}
};

#endif
