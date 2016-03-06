//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GUI_DOCUMENT_GAMEDOCUMENT_H_
#define _GUI_DOCUMENT_GAMEDOCUMENT_H_

#include "gui/guiDocument.h"

class GameDocument : public GuiDocument {
	bool getPaused();
public:
	GameDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document);

	virtual void onEvent(Rocket::Core::Event &event);

	virtual void onSleep();
	virtual void onWake();
	virtual void onRender();
};

#endif
