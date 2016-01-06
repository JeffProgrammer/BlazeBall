//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include "platformSDL/SDLWindow.h"
#include "platformSDL/SDLTimer.h"
#include "platform/network.h"
#include "render/scene.h"
#include "game/GameInterior.h"
#include "scriptEngine/scriptEngine.h"

// TODO: clean this shit up
#include "physics/bullet/btPhysicsEngine.h"
#include "game/levelLoader.h"
#include "network/server.h"

extern GLuint gSphereVBO;

void parseArgs(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {
	//Create us a new scene
	Scene *scene = Scene::getSingleton();

	//Load the physics engine
	PhysicsEngine::setEngine(new btPhysicsEngine());

	//Init SDL
	scene->mWindow = new SDLWindow();
	scene->mTimer = new SDLTimer();
	scene->mConfig = new Config("config.txt");

	//Init SDL
	if (!scene->init()) {
		return 1;
	}

	// Init script engine and call the main function
	if (!ScriptEngine::getSingleton()->init())
		return 1;

	// Load the networking engine
	Network::init();
	
	// parse command line arguments.
	parseArgs(argc, argv);

	// create a server obj
	Server s;
	s.start();

	//Let our scene go!
	scene->run();

	// stop server
	s.stop();

	// much hack, very wow
	if (gSphereVBO)
		glDeleteBuffers(1, &gSphereVBO);

	// destroy the networking engine
	Network::destroy();

	return 0;
}

void parseArgs(int argc, const char *argv[]) {

}