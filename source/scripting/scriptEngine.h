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

#ifndef scriptEngine_h
#define scriptEngine_h

#include <base/types.h>
#include "v8utils.h"

using namespace v8;

class ScriptingEngine {
protected:
	std::map<std::string, void(*)(const FunctionCallbackInfo<Value> &)> functions;

	template<typename T>
	inline void concat_array(std::vector<Local<Value>> &array, T first) {
		array.push_back(V8Utils::v8convert<std::string, Local<String>>(std::to_string(first)));
	}
	inline void concat_array(std::vector<Local<Value>> &array, const char *first) {
		array.push_back(V8Utils::v8convert<std::string, Local<String>>(std::string(first)));
	}
	template<typename T>
	inline void concat_array(std::vector<Local<Value>> &array, Local<T> first) {
		array.push_back(first);
	}
	template<typename T, typename... Args>
	inline void concat_array(std::vector<Local<Value>> &array, T first, Args... args) {
		concat_array(array, first);
		concat_array(array, args...);
	}
public:
	Isolate *isolate;
	Local<Context> context;

	ScriptingEngine();
	~ScriptingEngine();

	bool runScript(const std::string &script);
	bool runScript(const std::string &script, std::string &output);

	bool runScriptFile(const std::string &path);
	bool runScriptFile(const std::string &path, std::string &output);

	Local<String> call(const std::string &function);
	Local<String> call(const std::string &function, std::vector<Local<Value>> &args);
	Local<String> callArray(const std::string &function, Local<Array> array);
	Local<String> callValues(const std::string &function, U32 count, Local<Value> *values);
	template<typename... Args>
	inline Local<Value> call(const std::string &function, Args... args) {
		v8::EscapableHandleScope scope(isolate);
		std::vector<Local<Value>> array;
		concat_array(array, args...);

		return scope.Escape(callValues(function, array.size(), &array[0]));
	}

	void addFunction(const std::string &name, void(*callback)(const FunctionCallbackInfo<Value> &));
	void createContext();
};

#define SCRIPT_CREATE_SCOPE(isolate) \
	Isolate::Scope isolate_scope(isolate); \
	HandleScope handle_scope(isolate);
#define SCRIPT_CREATE_CONTEXT(context) \
	Context::Scope global_scope(context);

class ScriptFunctionConstructor {
public:
	std::string mName;
	void (*mFunction)(const FunctionCallbackInfo<Value> &);

	ScriptFunctionConstructor *next;
	static ScriptFunctionConstructor *last;

	ScriptFunctionConstructor(std::string name, void (*function)(const FunctionCallbackInfo<Value> &)) {
		next = last;
		last = this;

		mName = name;
		mFunction = function;
	}
};

#define SCRIPT_FUNCTION(name) \
void sf##name(const FunctionCallbackInfo<Value> &args); \
ScriptFunctionConstructor sfc##name(#name, sf##name); \
void sf##name(const FunctionCallbackInfo<Value> &args)

#endif