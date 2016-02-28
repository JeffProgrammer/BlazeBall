//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "guiDocument.h"

std::map<std::string, GuiDocument *> GuiDocument::sDocumentMap;

GuiDocument::GuiDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document) : mDocument(document), mRenderer(renderer) {
	sDocumentMap[name] = this;
	mListener = new Listener(this);
}

GuiDocument::~GuiDocument() {

}