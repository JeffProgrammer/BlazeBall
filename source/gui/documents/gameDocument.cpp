//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameDocument.h"
#include "render/renderer.h"

GameDocument::GameDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document) : GuiDocument(name, renderer, document) {
	mDocument->GetElementById("back")->AddEventListener("click", mListener);
}

void GameDocument::onEvent(Rocket::Core::Event &event) {
	if (event.GetType() == "click") {
		Rocket::Core::String command = event.GetCurrentElement()->GetAttribute("data-command", Rocket::Core::String("none"));

		if (command == "setContent") {
			Rocket::Core::String content = event.GetCurrentElement()->GetAttribute("data-content", Rocket::Core::String(""));

			GuiDocument *doc = GuiDocument::getDocument(content.CString());
			if (doc) {
				mRenderer->setCurrentDocument(doc);
			}
		}
	}
}

void GameDocument::onSleep() {

}

void GameDocument::onWake() {
	mRenderer->setCaptureMouse(true);
}
