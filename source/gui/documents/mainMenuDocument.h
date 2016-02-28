//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GUI_DOCUMENTS_MAINMENUDOCUMENT_H_
#define _GUI_DOCUMENTS_MAINMENUDOCUMENT_H_

#include "gui/guiDocument.h"

class MainMenuDocument : public GuiDocument {
public:
	MainMenuDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document);

	virtual void onEvent(Rocket::Core::Event &event);
};

#endif
