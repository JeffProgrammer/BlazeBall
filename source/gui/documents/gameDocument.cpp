//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameDocument.h"
#include "render/renderer.h"
#include "network/client.h"

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
		int key = event.GetParameter("key_identifier", 0);

		//Esc key hides the pause menu
		if (key == Rocket::Core::Input::KI_ESCAPE) {
			auto *pauseScreen = mDocument->GetElementById("pauseScreen");
			bool shouldHide = !pauseScreen->IsClassSet("hidden");

			pauseScreen->SetClass("hidden", shouldHide);

			//If we show the pause menu we shouldn't capture mouse
			mRenderer->setCaptureMouse(shouldHide);
		}

		Client *client = mRenderer->getClient();
		Config *config = client->getConfig();

		if (key == config->getKey("moveForward"))  client->getMovement().forward  = true;
		if (key == config->getKey("moveBackward")) client->getMovement().backward = true;
		if (key == config->getKey("moveLeft"))     client->getMovement().left     = true;
		if (key == config->getKey("moveRight"))    client->getMovement().right    = true;
	} else if (type == "keyup") {
		int key = event.GetParameter("key_identifier", 0);

		Client *client = mRenderer->getClient();
		Config *config = client->getConfig();

		if (key == config->getKey("moveForward"))  client->getMovement().forward  = false;
		if (key == config->getKey("moveBackward")) client->getMovement().backward = false;
		if (key == config->getKey("moveLeft"))     client->getMovement().left     = false;
		if (key == config->getKey("moveRight"))    client->getMovement().right    = false;
	}
}

void GameDocument::onSleep() {

}

void GameDocument::onWake() {
	mRenderer->setCaptureMouse(true);

	//Hide pause menu
	mDocument->GetElementById("pauseScreen")->SetClass("hidden", true);
}
