//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/Shape.h"
#include "render/modelManager.h"

Shape::Shape(World *world, const std::string &shapeFile) : RenderedObject(world) {
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