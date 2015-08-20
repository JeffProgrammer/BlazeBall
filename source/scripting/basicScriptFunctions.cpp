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

#include "scriptEngine.h"
#include "render/scene.h"

SCRIPT_FUNCTION(quit) {
	//Quit the game, duh
	exit(0);
}

SCRIPT_FUNCTION(print) {
	//Print all the args from the method in order
	for (U32 i = 0; i < args.Length(); i ++) {
		v8::HandleScope scope(args.GetIsolate());
		printf("%s", V8Utils::v8convert<Local<String>, std::string>(args[i]->ToString()).c_str());
	}
	//Newline at the end
	printf("\n");
}

SCRIPT_FUNCTION(require) {
	//Execute another script file and return its values
	std::string path = V8Utils::v8convert<Local<String>, std::string>(args[0]->ToString());

	std::string result;
	if (Scene::getSingleton()->mEngine->runScriptFile(path, result)) {
		args.GetReturnValue().Set(V8Utils::v8convert<std::string, Local<String>>(Scene::getSingleton()->mEngine->isolate, result));
	} else {
		args.GetReturnValue().SetEmptyString();
	}
}

namespace glm {
	EXTERN_OBJECT_CONSTRUCTOR(vec3, (0)) {
		switch (args.Length()) {
			case 1: {
				auto value = args[0]->ToNumber()->Value();
				object->x = value;
				object->y = value;
				object->z = value;
				break;
			}
			case 3: {
				object->x = args[0]->ToNumber()->Value();
				object->y = args[1]->ToNumber()->Value();
				object->z = args[2]->ToNumber()->Value();
				break;
			}
		}
	}

	EXTERN_OBJECT_METHOD(vec3, getX) {
		args.GetReturnValue().Set(Number::New(isolate, object->x));
	}
	EXTERN_OBJECT_METHOD(vec3, getY) {
		args.GetReturnValue().Set(Number::New(isolate, object->y));
	}
	EXTERN_OBJECT_METHOD(vec3, getZ) {
		args.GetReturnValue().Set(Number::New(isolate, object->z));
	}
	EXTERN_OBJECT_METHOD(vec3, setX) {
		object->x = args[0]->ToNumber()->Value();
	}
	EXTERN_OBJECT_METHOD(vec3, setY) {
		object->x = args[1]->ToNumber()->Value();
	}
	EXTERN_OBJECT_METHOD(vec3, setZ) {
		object->x = args[2]->ToNumber()->Value();
	}
}
