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
#include "base/io.h"

ScriptFunctionConstructor *ScriptFunctionConstructor::last = nullptr;

ScriptingEngine::ScriptingEngine() {
	// Initialize V8.
	V8::InitializeICU();
	Platform *platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();

	// Create a new Isolate and make it the current one.
	ArrayBufferAllocator *allocator = new ArrayBufferAllocator;
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = allocator;
	isolate = Isolate::New(create_params);
}
ScriptingEngine::~ScriptingEngine() {
	V8::Dispose();
	V8::ShutdownPlatform();
}

void ScriptingEngine::createContext() {
	//This needs to be in two separate things so it can be copied into global
	Local<ObjectTemplate> otemp = ObjectTemplate::New(isolate);

	//Add functions which were stored in a list
	for (auto pair : functions) {
		auto name = pair.first;
		auto callback = pair.second;

		Local<String> lname = V8Utils::v8convert<std::string, Local<String>>(isolate, name);
		Local<FunctionTemplate> lfunc = FunctionTemplate::New(isolate, callback);

		otemp->Set(lname, lfunc);
	}

	//Also add functions created with the function constructors
	for (ScriptFunctionConstructor *start = ScriptFunctionConstructor::last; start; start = start->next) {
		auto name = start->mName;
		auto callback = start->mFunction;

		Local<String> lname = V8Utils::v8convert<std::string, Local<String>>(isolate, name);
		Local<FunctionTemplate> lfunc = FunctionTemplate::New(isolate, callback);

		otemp->Set(lname, lfunc);
	}

	//Two lines, same as above
	context = Context::New(isolate, NULL, otemp);
}

bool ScriptingEngine::runScript(const std::string &script) {
	std::string temp; //So we can satisfy the method below; ignored
	return runScript(script, temp);
}

bool ScriptingEngine::runScript(const std::string &script, std::string &output) {
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

bool ScriptingEngine::runScriptFile(const std::string &path) {
	std::string output;
	return runScriptFile(path, output);
}

bool ScriptingEngine::runScriptFile(const std::string &path, std::string &output) {
	//Get the contents of the file

	U32 length;
	U8 *conts = IO::readFile(path, length);
	if (conts == NULL) {
		output = "Error: File \"" + path + "\" not found";
		return false;
	}

	std::string str(reinterpret_cast<const char *>(conts));
	delete [] conts;

	return runScript(str, output);
}

Local<String> ScriptingEngine::call(const std::string &function) {
	EscapableHandleScope scope(isolate);
	Local<Value> values[] = {};
	return scope.Escape(callValues(function, 0, values));
}

Local<String> ScriptingEngine::call(const std::string &function, std::vector<Local<Value>> &args) {
	EscapableHandleScope scope(isolate);
	return scope.Escape(callValues(function, args.size(), &args[0]));
}

Local<String> ScriptingEngine::callValues(const std::string &function, U32 count, Local<Value> *values) {
	EscapableHandleScope scope(isolate);

	Local<Context> func_context = Local<Context>::New(isolate, context);
	Context::Scope context_scope(func_context);

	Local<Value> func;
	if (!func_context->Global()->Get(func_context, V8Utils::v8convert<std::string, Local<String>>(isolate, function)).ToLocal(&func) || !func->IsFunction()) {
		return scope.Escape(V8Utils::v8convert<const char *, Local<String>>(isolate, ""));
	}

	Local<Function> actual_func = func.As<Function>();

	MaybeLocal<Value> value = actual_func->Call(func_context, func_context->Global(), count, values);
	if (value.IsEmpty()) {
		return scope.Escape(V8Utils::v8convert<const char *, Local<String>>(isolate, ""));
	}

	Local<Value> outValue;
	if (!value.ToLocal(&outValue)) {
		return scope.Escape(V8Utils::v8convert<const char *, Local<String>>(isolate, ""));
	}

	return scope.Escape(outValue->ToString());
}

void ScriptingEngine::addFunction(const std::string &name, void(*callback)(const FunctionCallbackInfo<Value> &)) {
	functions[name] = callback;
}
