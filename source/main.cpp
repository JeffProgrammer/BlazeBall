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