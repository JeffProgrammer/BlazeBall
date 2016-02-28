//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GUI_DOCUMENT_GAMEDOCUMENT_H_
#define _GUI_DOCUMENT_GAMEDOCUMENT_H_

#include "gui/guiDocument.h"

class GameDocument : public GuiDocument {
public:
	GameDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document) : GuiDocument(name, renderer, document) {}
};

#endif
