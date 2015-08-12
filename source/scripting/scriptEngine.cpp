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
#include "v8utils.h"

using namespace v8;

struct V8State {
	V8State() {
		mInitialized = false;
	}

	bool mInitialized;
	Platform *mPlatform;
	Isolate *mGlobalIsolate;
	Local<ObjectTemplate> mGlobalObject;
	Local<Context> mGlobalContext;
	Context::Scope *mGlobalScope;

	bool getInitialized() { return mInitialized; }
	Isolate *getIsolate() { return mGlobalIsolate; }
	const Local<ObjectTemplate> &getGlobal() { return mGlobalObject; }
	const Local<Context> &getContext() { return mGlobalContext; }
	Context::Scope *getScope() { return mGlobalScope; }
};

ScriptingEngine::ScriptingEngine() {
	mState = new V8State;
}
ScriptingEngine::~ScriptingEngine() {
	delete mState;
}

bool ScriptingEngine::runScript(const std::string &script, std::string &output) {
	if (!mState->getInitialized()) {
		initContext();
	}

	// Create a string containing the JavaScript source code.
	Local<String> source = V8Utils::v8convert<std::string, Local<String>>(mState->getIsolate(), script);

	// Compile the source code.
	Local<Script> compiled;
	TryCatch try_catch(mState->getIsolate());

	if (!Script::Compile(mState->getContext(), source).ToLocal(&compiled)) {
		V8Utils::ReportException(mState->getIsolate(), &try_catch);
		return false;
	} else {
		// Run the script to get the result.
		Local<Value> result;
		if (!compiled->Run(mState->getContext()).ToLocal(&result)) {
			V8Utils::ReportException(mState->getIsolate(), &try_catch);
			return false;
		} else {
			// Convert the result to an UTF8 string and print it.
			String::Utf8Value utf8(result);
			output = V8Utils::v8convert<V8Utils::v8StrValue, std::string>(utf8);
			return true;
		}
	}
}

void ScriptingEngine::initContext() {
	// Initialize V8.
	V8::InitializeICU();
	mState->mPlatform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(mState->mPlatform);
	V8::Initialize();

	mState->mInitialized = true;

	// Create a new Isolate and make it the current one.
	ArrayBufferAllocator allocator;
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = &allocator;
	mState->mGlobalIsolate = Isolate::New(create_params);
	Isolate::Scope isolate_scope(mState->getIsolate());

	// Create a stack-allocated handle scope.
	HandleScope handle_scope(mState->getIsolate());

	mState->mGlobalObject = ObjectTemplate::New(mState->getIsolate());
	//TODO: add functions

	// Create a new context.
	mState->mGlobalContext = Context::New(mState->getIsolate(), NULL, mState->getGlobal());

	// Enter the context for compiling and running the hello world script.
	mState->mGlobalScope = new Context::Scope(mState->getContext());
}
void ScriptingEngine::destroyContext() {
	delete mState->mGlobalScope;

	// Dispose the isolate and tear down V8.
	mState->mGlobalIsolate->Dispose();
	V8::Dispose();
	V8::ShutdownPlatform();
	delete mState->mPlatform;
}

