//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/Shape.h"
#include "render/modelManager.h"

Shape::Shape(const std::string &shapeFile) {
	mShapeFile = shapeFile;
}

Shape::~Shape() {
}

void Shape::updateTick(const F64 &delta) {

}

void Shape::render(RenderInfo &info) {
	
}

void Shape::loadShape() {
	bool loaded = MODELMGR->loadAsset(mShapeFile);
	MODELMGR->addShape(this);
}