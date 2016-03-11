//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "mainMenuDocument.h"
#include "render/renderer.h"
#include "main/gameState.h"

MainMenuDocument::MainMenuDocument(const std::string &name, Renderer *renderer, Rocket::Core::ElementDocument *document) : GuiDocument(name, renderer, document) {
	mDocument->GetElementById("play")->AddEventListener("click", mListener);
	mDocument->GetElementById("game")->AddEventListener("click", mListener);
}

void MainMenuDocument::onEvent(Rocket::Core::Event &event) {
	if (event.GetType() == "click") {
		Rocket::Core::String command = event.GetCurrentElement()->GetAttribute("data-command", Rocket::Core::String("none"));

		if (command == "setContent") {
			Rocket::Core::String content = event.GetCurrentElement()->GetAttribute("data-content", Rocket::Core::String(""));

			GuiDocument *doc = GuiDocument::getDocument(content.CString());
			if (doc) {
				mRenderer->setCurrentDocument(doc);
			}
		} else if (command == "loadServer") {
			Rocket::Core::String content = event.GetCurrentElement()->GetAttribute("data-content", Rocket::Core::String(""));

			//Load up a server
			GameState::gState->createServer(28000);
			GameState::gState->server->loadLevel(content.CString());

			//Connect to it
			GameState::gState->connectToServer("localhost", 28000);
		}
	}
}

void MainMenuDocument::onSleep() {

}
void MainMenuDocument::onWake() {
	mRenderer->setCaptureMouse(false);
}