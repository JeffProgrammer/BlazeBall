//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <libgen.h>
#include <unistd.h>
#include "scene.h"


int main(int argc, const char * argv[])
{
	//Usage prompt
	if (argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	Physics::getPhysics()->init();

	U32 argstart = 1;

	if (!strcmp(argv[1], "-o")) {
		argstart += 2;
	}
	if (!strcmp(argv[1], "-c")) {
		argstart += 1;
	}

	Scene *scene = Scene::getSingleton();

	scene->difCount = 0;
	scene->difs = new DIF*[argc - argstart];
	scene->filenames = new String*[argc - argstart];

	for (U32 i = 0; i < (argc - argstart); i ++) {
		String directory = String(dirname((char *)argv[i + argstart]));

		//Open file
		FILE *file = fopen(argv[i + argstart], "r");

		//Read the .dif
		scene->difs[i] = new DIF(file, directory);
		if (scene->difs[i]) {
			scene->difCount ++;
		}

		//Clean up
		fclose(file);

		scene->filenames[i] = new String(argv[i + argstart]);
	}

	if (!strcmp(argv[1], "-o")) {
		FILE *out = fopen(argv[2], "w");
		scene->difs[0]->interior[0]->exportObj(out);
		fflush(out);
		fclose(out);
	} else if (!strcmp(argv[1], "-c")) {
		for (U32 i = 0; i < scene->difCount; i ++) {
			String directory = String(dirname((char *)scene->filenames[i]));

			FILE *output = fopen((const char *)scene->filenames[i], "w");
			scene->difs[i]->write(output, directory);
			fflush(output);
			fclose(output);
		}
	} else {
		//Init SDL and go!
		Scene::getSingleton()->run();
	}

	return 0;
}
