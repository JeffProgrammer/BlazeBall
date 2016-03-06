//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameDocument.h"
#include "render/renderer.h"

GameDocument::GameDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document) : GuiDocument(name, renderer, document) {
	mDocument->GetElementById("back")->AddEventListener("click", mListener);
	mDocument->AddEventListener("keydown", mListener);
	mDocument->AddEventListener("keyup", mListener);
}

void GameDocument::onEvent(Rocket::Core::Event &event) {
	Rocket::Core::String type = event.GetType();
	if (type == "click") {
		Rocket::Core::String command = event.GetCurrentElement()->GetAttribute("data-command", Rocket::Core::String("none"));

		if (command == "setContent") {
			Rocket::Core::String content = event.GetCurrentElement()->GetAttribute("data-content", Rocket::Core::String(""));

			GuiDocument *doc = GuiDocument::getDocument(content.CString());
			if (doc) {
				mRenderer->setCurrentDocument(doc);
			}
		}
	} else if (type == "keydown") {
		int keyIdentifier = event.GetParameter("key_identifier", 0);

		//Esc key hides the pause menu
		if (keyIdentifier == Rocket::Core::Input::KI_ESCAPE) {
			auto *pauseScreen = mDocument->GetElementById("pauseScreen");
			bool shouldHide = !pauseScreen->IsClassSet("hidden");

			pauseScreen->SetClass("hidden", shouldHide);

			//If we show the pause menu we shouldn't capture mouse
			mRenderer->setCaptureMouse(shouldHide);
		}
		//TODO Marble control
	} else if (type == "keyup") {
		int keyIdentifier = event.GetParameter("key_identifier", 0);
		//TODO Marble control
	}
}

void GameDocument::onSleep() {

}

void GameDocument::onWake() {
	mRenderer->setCaptureMouse(true);

	//Hide pause menu
	mDocument->GetElementById("pauseScreen")->SetClass("hidden", true);
}
