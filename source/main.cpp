//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <fstream>
#include "platformSDL/SDLWindow.h"
#include "platformSDL/SDLTimer.h"
#include "render/scene.h"
#include "scripting/scriptEngine.h"
#include "physics/bullet/btPhysicsEngine.h"

int main(int argc, const char *argv[]) {
	//Create us a new scene
	Scene *scene = Scene::getSingleton();

	//Create the scripting engine
	ScriptingEngine *scripting = new ScriptingEngine();
	scene->mEngine = scripting;

	//Load up the scope and context of the scripting system
	SCRIPT_CREATE_SCOPE(scripting->isolate);
	scripting->createContext();
	SCRIPT_CREATE_CONTEXT(scripting->context);

	//Load the physics engine
	PhysicsEngine::setEngine(new btPhysicsEngine());

	//Init SDL
	scene->window = new SDLWindow();
	scene->mTimer = new SDLTimer();

	//Load up the main script file which has all the starting stuff
	scripting->runScriptFile("main.js");

	//Load up the arguments and let javascript parse them
	v8::Local<v8::Array> args = V8Utils::createStringArray(scripting->isolate, argc, argv);
	std::string out = V8Utils::v8convert<Local<String>, std::string>(scripting->call("parseArgs", args)->ToString(scripting->isolate));

	//Start it up
	scripting->call("onStart");

	//Let our scene go!
	scene->run();

	return 0;
}
