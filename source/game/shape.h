//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
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
	virtual void updateTick(const F64 &delta) override;
	virtual void render(RenderInfo &info) override;

	void loadShape();

	const std::string& getShapeFile() const {
		return mShapeFile;
	}
};

#endif
