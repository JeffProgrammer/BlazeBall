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
		array.push_back(V8Utils::v8convert<std::string, Local<String>>(isolate, std::to_string(first)));
	}
	inline void concat_array(std::vector<Local<Value>> &array, const char *first) {
		array.push_back(V8Utils::v8convert<const char *, Local<String>>(isolate, first));
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

	static std::map<std::string, Global<FunctionTemplate>> objectConstructors;

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

	static Local<Object> instantiateClass(Isolate *isolate, const std::string &name);
};

typedef void(*v8callback)(const FunctionCallbackInfo<Value> &);

#define SCRIPT_CREATE_SCOPE(isolate) \
	Isolate::Scope isolate_scope(isolate); \
	HandleScope handle_scope(isolate);
#define SCRIPT_CREATE_CONTEXT(context) \
	Context::Scope global_scope(context);

class ScriptFunctionConstructor {
public:
	std::string mName;
	v8callback mFunction;

	ScriptFunctionConstructor *next;
	static ScriptFunctionConstructor *last;

	ScriptFunctionConstructor(const std::string &name, v8callback function) {
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

class ScriptClassConstructor {
public:
	std::string mName;
	v8callback mConstructor;
	std::vector<std::pair<std::string, v8callback> > mMethods;

	ScriptClassConstructor *next;
	static ScriptClassConstructor *last;

	ScriptClassConstructor(const std::string &name, v8callback constructor) {
		next = last;
		last = this;

		mName = name;
		mConstructor = constructor;
	}

	void addMethod(const std::string &name, v8callback callback) {
		mMethods.push_back(std::make_pair(name, callback));
	}

	class MethodConstructor {
	public:
		MethodConstructor(const std::string &name, v8callback function, ScriptClassConstructor *parent) {
			parent->addMethod(name, function);
		}
	};
};

#define OBJECT_CONSTRUCTOR(name, constructor) \
	static void __oc##name(const FunctionCallbackInfo<Value> &args) { \
		HandleScope scope(args.GetIsolate()); \
		if (args.This()->InternalFieldCount() == 0) { \
			args.GetReturnValue().SetUndefined(); \
			return; \
		} \
		\
		name *object = new name constructor; \
		__occ##name(object, args); \
		\
		object->Wrap(args.This()); \
		args.GetReturnValue().Set(args.This()); \
	} \
	static ScriptClassConstructor *__scc##name; \
	static void __occ##name(name *object, const v8::FunctionCallbackInfo<v8::Value> &args)

#define IMPLEMENT_OBJECT(name) \
	ScriptClassConstructor *name::__scc##name = new ScriptClassConstructor(#name, __oc##name)

#define OBJECT_METHOD(classname, name) \
	void __m##classname##name(classname *object, Isolate *isolate, const FunctionCallbackInfo<Value> &args); \
	static void __mc##classname##name(const FunctionCallbackInfo<Value> &args) { \
		classname *object = ObjectWrap::Unwrap<classname>(args.This()); \
		__m##classname##name(object, args.GetIsolate(), args); \
	} \
	ScriptClassConstructor::MethodConstructor mc##classname##name(#name, __mc##classname##name, classname::__scc##classname); \
	void __m##classname##name(classname *object, Isolate *isolate, const FunctionCallbackInfo<Value> &args)

#define EXTERN_OBJECT_CONSTRUCTOR(name, constructor) \
	class ext_##name : public ObjectWrap { \
	public: \
		name *mHandle; \
	protected: \
		inline ~ext_##name () { \
			delete mHandle; \
		} \
	}; \
	static void __occ##name(name *object, const v8::FunctionCallbackInfo<v8::Value> &args); \
	static void __oc##name(const FunctionCallbackInfo<Value> &args) { \
		HandleScope scope(args.GetIsolate()); \
		if (args.This()->InternalFieldCount() == 0) { \
			args.GetReturnValue().SetUndefined(); \
			return; \
		} \
		\
		ext_##name *object = new ext_##name (); \
		object->mHandle = new name constructor; \
		__occ##name(object->mHandle, args); \
		\
		object->Wrap(args.This()); \
		args.GetReturnValue().Set(args.This()); \
	} \
	static ScriptClassConstructor *__scc##name = new ScriptClassConstructor(#name, __oc##name); \
	static void __occ##name(name *object, const v8::FunctionCallbackInfo<v8::Value> &args) \

#define EXTERN_OBJECT_METHOD(classname, name) \
	void __m##classname##name(classname *object, Isolate *isolate, const FunctionCallbackInfo<Value> &args); \
	static void __mc##classname##name(const FunctionCallbackInfo<Value> &args) { \
		ext_##classname *object = ObjectWrap::Unwrap<ext_##classname>(args.This()); \
		__m##classname##name(object->mHandle, args.GetIsolate(), args); \
	} \
	ScriptClassConstructor::MethodConstructor mc##classname##name(#name, __mc##classname##name, __scc##classname); \
	void __m##classname##name(classname *object, Isolate *isolate, const FunctionCallbackInfo<Value> &args)

#define OBJECT(classname, name) \
	Local<Object> name = ScriptingEngine::instantiateClass(isolate, #classname); \
	c_##name->Wrap(name); \
	classname *c_##name

#define EXTERN_OBJECT(classname, name) \
	ext_##classname *c_##name = new ext_##classname; \
	Local<Object> name = ScriptingEngine::instantiateClass(isolate, #classname); \
	c_##name->Wrap(name); \
	c_##name->mHandle

#define UNWRAP(classname, thing) \
	ObjectWrap::Unwrap<classname>(thing->ToObject())

#define UNWRAP_EXTERN(classname, thing) \
	ObjectWrap::Unwrap<ext_##classname>(thing->ToObject())->mHandle

#endif
