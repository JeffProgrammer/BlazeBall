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
ScriptingEngine::ScriptingEngine() {
	// Initialize V8.
	V8::InitializeICU();
	Platform *platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();
}
ScriptingEngine::~ScriptingEngine() {
	V8::Dispose();
	V8::ShutdownPlatform();
}

Local<Context> ScriptingEngine::createContext(Isolate *isolate) {
	//This needs to be in two separate things so it can be copied into global
	Local<ObjectTemplate> otemp = ObjectTemplate::New(isolate);

	//TODO: add functions
	for (auto pair : functions) {
		auto name = pair.first;
		auto callback = pair.second;

		Local<String> lname = V8Utils::v8convert<std::string, Local<String>>(isolate, name);
		Local<FunctionTemplate> lfunc = FunctionTemplate::New(isolate, callback);

		otemp->Set(lname, lfunc);
	}

	//Two lines, same as above
	return Context::New(isolate, NULL, otemp);
}

bool ScriptingEngine::runScript(Isolate *isolate, Local<Context> context,const std::string &script) {
	std::string temp; //So we can satisfy the method below; ignored
	return runScript(isolate, context, script, temp);
}

bool ScriptingEngine::runScript(Isolate *isolate, Local<Context> context,const std::string &script, std::string &output) {
	Local<String> source = V8Utils::v8convert<std::string, Local<String>>(isolate, script);

	// Compile the source code.
	Local<Script> compiled;
	TryCatch try_catch(isolate);

	if (!Script::Compile(context, source).ToLocal(&compiled)) {
		V8Utils::ReportException(isolate, &try_catch);
		return false;
	} else {
		// Run the script to get the result.
		Local<Value> result;
		if (!compiled->Run(context).ToLocal(&result)) {
			V8Utils::ReportException(isolate, &try_catch);
			return false;
		} else {
			// Convert the result to an UTF8 string and print it.
			Local<String> utf8 = result->ToString(isolate);
			output = V8Utils::v8convert<Local<String>, std::string>(utf8);
			return true;
		}
	}
	return false;
}

void ScriptingEngine::addFunction(const std::string &name, void(*callback)(const FunctionCallbackInfo<Value> &)) {
	functions[name] = callback;
}

