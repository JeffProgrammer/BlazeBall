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
	mDocument->AddEventListener("mousemove", mListener);
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

		if (!getPaused()) {
			Client *client = mRenderer->getClient();
			Config *config = client->getConfig();

			//TODO: Make this neater
			if (key == config->getKey("moveForward"))  client->getMovement().forward  = true;
			if (key == config->getKey("moveBackward")) client->getMovement().backward = true;
			if (key == config->getKey("moveLeft"))     client->getMovement().left     = true;
			if (key == config->getKey("moveRight"))    client->getMovement().right    = true;

			if (key == config->getKey("lookUp"))    client->getMovement().pitchUp   = true;
			if (key == config->getKey("lookDown"))  client->getMovement().pitchDown = true;
			if (key == config->getKey("lookLeft"))  client->getMovement().yawLeft   = true;
			if (key == config->getKey("lookRight")) client->getMovement().yawRight  = true;
			if (key == config->getKey("jump"))      client->getMovement().jump      = true;
			if (key == config->getKey("fire"))      client->getMovement().fire      = true;
		}
	} else if (type == "keyup") {
		int key = event.GetParameter("key_identifier", 0);

		if (!getPaused()) {
			Client *client = mRenderer->getClient();
			Config *config = client->getConfig();

			//TODO: Make this neater
			if (key == config->getKey("moveForward"))  client->getMovement().forward  = false;
			if (key == config->getKey("moveBackward")) client->getMovement().backward = false;
			if (key == config->getKey("moveLeft"))     client->getMovement().left     = false;
			if (key == config->getKey("moveRight"))    client->getMovement().right    = false;

			if (key == config->getKey("lookUp"))    client->getMovement().pitchUp   = false;
			if (key == config->getKey("lookDown"))  client->getMovement().pitchDown = false;
			if (key == config->getKey("lookLeft"))  client->getMovement().yawLeft   = false;
			if (key == config->getKey("lookRight")) client->getMovement().yawRight  = false;
			if (key == config->getKey("jump"))      client->getMovement().jump      = false;
			if (key == config->getKey("fire"))      client->getMovement().fire      = false;
		}
	} else if (type == "mousemove") {
		//Only update movement if we're not paused
		if (!getPaused()) {
			int x = event.GetParameter("movement_x", 0);
			int y = event.GetParameter("movement_y", 0);

			Client *client = mRenderer->getClient();

			client->getMovement().yaw = static_cast<F32>(x);
			client->getMovement().pitch = static_cast<F32>(y);
		}
	}
}

bool GameDocument::getPaused() {
	return !mRenderer->getCaptureMouse();
}

void GameDocument::onSleep() {

}

void GameDocument::onWake() {
	mRenderer->setCaptureMouse(true);

	//Hide pause menu
	mDocument->GetElementById("pauseScreen")->SetClass("hidden", true);
}

void GameDocument::onRender() {
	Rocket::Core::String str;
	str.FormatString(32, "FPS: %.1f", mRenderer->getFPS());
	mDocument->GetElementById("fpsMeter")->SetInnerRML(str);
}