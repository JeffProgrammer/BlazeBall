//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include "platformSDL/SDLWindow.h"
#include "platformSDL/SDLTimer.h"
#include "render/scene.h"
#include "game/GameInterior.h"
#include "scriptEngine/scriptEngine.h"

#include "physics/bullet/btPhysicsEngine.h"
#include "game/levelLoader.h"

extern GLuint gSphereVBO;

void parseArgs(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {
	//Create us a new scene
	Scene *scene = Scene::getSingleton();

	//Load the physics engine
	PhysicsEngine::setEngine(new btPhysicsEngine());

	//Init SDL
	scene->window = new SDLWindow();
	scene->mTimer = new SDLTimer();
	scene->mConfig = new Config("config.txt");

	//Init SDL
	if (!scene->init()) {
		return 1;
	}

	// Init script engine and call the main function
	if (!ScriptEngine::getSingleton()->init())
		return 1;

	// parse command line arguments.
	parseArgs(argc, argv);

	//Let our scene go!
	scene->run();

	// much hack, very wow
	if (gSphereVBO)
		glDeleteBuffers(1, &gSphereVBO);

	return 0;
}

void parseArgs(int argc, const char *argv[]) {
	if (argc == 1) {
		printf("Must specify a json file to load.\n");
		assert(false);
	}
	//loadLevel(argv[1]); // suck it nerds
}