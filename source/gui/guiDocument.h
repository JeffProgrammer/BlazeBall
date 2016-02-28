//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GUI_GUIDOCUMENT_H_
#define _GUI_GUIDOCUMENT_H_

#include "base/types.h"

#include <Rocket/Core.h>
#include <Rocket/Core/Input.h>
#include <Rocket/Core/ElementDocument.h>

class Renderer;

class GuiDocument {
protected:
	class Listener : public Rocket::Core::EventListener {
		GuiDocument *mDocument;
	public:
		Listener(GuiDocument *document) : mDocument(document) {}
		virtual void ProcessEvent(Rocket::Core::Event &event) {
			mDocument->onEvent(event);
		}
	};

	Rocket::Core::ElementDocument *mDocument;
	Listener *mListener;
	Renderer *mRenderer;

	static std::map<std::string, GuiDocument *> sDocumentMap;
public:
	GuiDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document);
	virtual ~GuiDocument();

	virtual void onWake() {}
	virtual void onSleep() {}

	virtual void onEvent(Rocket::Core::Event &event) {}

	Rocket::Core::ElementDocument *getDocument() { return mDocument; }

	static GuiDocument *getDocument(const std::string &name) { return sDocumentMap[name]; }
};

#endif
