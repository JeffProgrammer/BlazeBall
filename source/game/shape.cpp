//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/Shape.h"
#include "render/modelManager.h"

Shape::Shape(const std::string &shapeFile) : RenderedObject() {
	mShapeFile = shapeFile;
}

Shape::~Shape() {
}

void Shape::updateTick(const F64 &delta) {

}

void Shape::render(RenderInfo &info) {
	
}

void Shape::loadShape() {
	MODELMGR->loadAsset(mShapeFile);
	MODELMGR->addShape(this);
}