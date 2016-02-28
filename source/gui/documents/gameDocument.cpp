//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameDocument.h"
#include "render/renderer.h"

void GameDocument::onSleep() {

}

void GameDocument::onWake() {
	mRenderer->setCaptureMouse(true);
}