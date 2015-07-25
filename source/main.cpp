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
#include "render/scene.h"
#include "game/gameInterior.h"

#include "physics/bullet/btPhysicsEngine.h"


int main(int argc, const char * argv[])
{
	//Usage prompt
	if (argc < 2) {
		printf("Usage: %s <files ...>\n", argv[0]);
		return 1;
	}

	PhysicsEngine::setEngine(new btPhysicsEngine());
	Scene *scene = Scene::getSingleton();

	for (U32 i = 1; i < argc; i ++) {
		std::string directory = IO::getPath(argv[i]);

		std::ifstream file(argv[i]);

		//Read the .dif
		DIF::DIF dif;
		if (dif.read(file)) {
			for (auto dinterior : dif.interior) {
				GameInterior *interior = new GameInterior(dinterior);
				interior->generateMaterials(directory);
				interior->generateMesh();
				scene->interiors.push_back(interior);
			}
		}

		//Clean up
		file.close();
	}
	//Init SDL and go!
	scene->window = new SDLWindow();
	scene->run();

	return 0;
}
