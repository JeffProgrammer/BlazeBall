//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <fstream>
#include "platformSDL/SDLWindow.h"
#include "platformSDL/SDLTimer.h"
#include "script/scriptEngine.h"
#include "render/scene.h"
#include "physics/bullet/btPhysicsEngine.h"
#include "game/GameInterior.h"

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

	//Init SDL
	if (!scene->init()) {
		return 1;
	}

	// Init script engine
	SCRIPTENGINE->init();

	// parse command line arguments.
	parseArgs(argc, argv);

	//Let our scene go!
	scene->run();

	// much hack, very wow
	if (gSphereVBO)
		glDeleteBuffers(1, &gSphereVBO);

	SCRIPTENGINE->destroy();

	return 0;
}

void parseArgs(int argc, const char *argv[]) {
	for (int i = 1; i < argc; i++) {
		DIF::DIF dif;

		std::string path = argv[1];
		std::string directory = IO::getPath(path);
		std::ifstream file(path, std::ios::binary);

		if (dif.read(file)) {
			GameInterior *interior = new GameInterior();

			// load interior
			for (auto dinterior : dif.interior) {
				interior->setInterior(dinterior);
				interior->generateMaterials(directory);
			}

			// make mesh
			interior->generateMesh();

			// add the interior to the scene.
			Scene::getSingleton()->addObject(interior);
		}

		// cleanup
		file.close();
	}
}