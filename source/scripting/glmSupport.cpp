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

EXTERN_OBJECT_CONSTRUCTOR(glm::vec3, (0)) {
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

EXTERN_OBJECT_METHOD(glm::vec3, getX) {
	args.GetReturnValue().Set(Number::New(isolate, object->x));
}
EXTERN_OBJECT_METHOD(glm::vec3, getY) {
	args.GetReturnValue().Set(Number::New(isolate, object->y));
}
EXTERN_OBJECT_METHOD(glm::vec3, getZ) {
	args.GetReturnValue().Set(Number::New(isolate, object->z));
}
EXTERN_OBJECT_METHOD(glm::vec3, setX) {
	object->x = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::vec3, setY) {
	object->y = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::vec3, setZ) {
	object->z = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::vec3, set) {
	object->x = args[0]->ToNumber()->Value();
	object->y = args[1]->ToNumber()->Value();
	object->z = args[2]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::vec3, length) {
	args.GetReturnValue().Set(glm::length(*object));
}
EXTERN_OBJECT_METHOD(glm::vec3, dot) {
	//Second vector
	glm::vec3 *object2 = UNWRAP_EXTERN(glm::vec3, args[0]);
	//Dot them and set that as the return value
	args.GetReturnValue().Set(Number::New(isolate, glm::dot(*object, *object2)));
}
EXTERN_OBJECT_METHOD(glm::vec3, cross) {
	//Second vector
	glm::vec3 *object2 = UNWRAP_EXTERN(glm::vec3, args[0]);

	//The cross product is another vector, instantiate it.
	EXTERN_OBJECT(glm::vec3, retobj) = new glm::vec3(glm::cross(*object, *object2));

	//And return it
	args.GetReturnValue().Set(retobj);
}
EXTERN_OBJECT_METHOD(glm::vec3, toString) {
	std::string out = "{" + std::to_string(object->x) + ", " + std::to_string(object->y) + ", " + std::to_string(object->z) + "}";
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, out.c_str()));
}

EXTERN_OBJECT_METHOD(glm::vec3, add) {
	glm::vec3 *object2 = UNWRAP_EXTERN(glm::vec3, args[0]);
	EXTERN_OBJECT(glm::vec3, retobj) = new glm::vec3(*object + *object2);
	args.GetReturnValue().Set(retobj);
}
EXTERN_OBJECT_METHOD(glm::vec3, sub) {
	glm::vec3 *object2 = UNWRAP_EXTERN(glm::vec3, args[0]);
	EXTERN_OBJECT(glm::vec3, retobj) = new glm::vec3(*object - *object2);
	args.GetReturnValue().Set(retobj);
}


EXTERN_OBJECT_CONSTRUCTOR(glm::quat, (0, glm::vec3(0))) {
	switch (args.Length()) {
		case 1: {
			auto value = args[0]->ToNumber()->Value();
			object->w = value;
			object->x = value;
			object->y = value;
			object->z = value;
			break;
		}
		case 4: {
			object->w = args[0]->ToNumber()->Value();
			object->x = args[1]->ToNumber()->Value();
			object->y = args[2]->ToNumber()->Value();
			object->z = args[3]->ToNumber()->Value();
			break;
		}
	}
}
EXTERN_OBJECT_METHOD(glm::quat, getW) {
	args.GetReturnValue().Set(Number::New(isolate, object->w));
}
EXTERN_OBJECT_METHOD(glm::quat, getX) {
	args.GetReturnValue().Set(Number::New(isolate, object->x));
}
EXTERN_OBJECT_METHOD(glm::quat, getY) {
	args.GetReturnValue().Set(Number::New(isolate, object->y));
}
EXTERN_OBJECT_METHOD(glm::quat, getZ) {
	args.GetReturnValue().Set(Number::New(isolate, object->z));
}
EXTERN_OBJECT_METHOD(glm::quat, setW) {
	object->w = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::quat, setX) {
	object->x = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::quat, setY) {
	object->y = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::quat, setZ) {
	object->z = args[0]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::quat, set) {
	object->w = args[0]->ToNumber()->Value();
	object->x = args[1]->ToNumber()->Value();
	object->y = args[2]->ToNumber()->Value();
	object->z = args[3]->ToNumber()->Value();
}
EXTERN_OBJECT_METHOD(glm::quat, length) {
	args.GetReturnValue().Set(glm::length(*object));
}
