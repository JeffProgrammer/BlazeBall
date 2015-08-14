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

#ifndef v8utils_h
#define v8utils_h

#include <base/types.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	inline virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	inline virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
	inline virtual void Free(void* data, size_t) { free(data); }
};

namespace V8Utils {
	template <typename from, typename to>
	inline to v8convert(const from &value);

	//V8 methods need an isolate
	template <typename from, typename to>
	inline to v8convert(v8::Isolate *isolate, from value);

	//Convert a v8 string into a c string
	template<>
	inline const char *v8convert(const v8::String::Utf8Value &value) {
		return *value ? *value : "<string conversion failed>";
	}

	template<>
	inline std::string v8convert(const v8::Local<v8::String> &value) {
		char *buffer = new char[value->Length() + 1];
		value->WriteUtf8(buffer);
		std::string ret(buffer);
		delete [] buffer;
		return ret;
	}

	template<>
	inline const char *v8convert(const v8::Local<v8::String> &value) {
		return v8convert<v8::Local<v8::String>, std::string>(value).c_str();
	}

	//Convert a c string into a v8 string
	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, const char *value) {
		return v8::String::NewFromUtf8(isolate, value);
	}

	template<>
	inline std::string v8convert(const v8::String::Utf8Value &value) {
		return std::string(*value);
	}

	template<typename T1, typename T2>
	inline v8::Local<T1> v8convert(T2 value) {
		//Use the isolate version, dumbass
		assert(false);
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, const std::string &value) {
		return v8::String::NewFromUtf8(isolate, value.c_str());
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, std::string value) {
		return v8::String::NewFromUtf8(isolate, value.c_str());
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, v8::Local<v8::Number> value) {
		return value->ToString(isolate);
	}
	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, v8::Local<v8::Integer> value) {
		return value->ToString(isolate);
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, S32 value) {
		return v8convert<v8::Local<v8::Integer>, v8::Local<v8::String>>(isolate, v8::Integer::New(isolate, value));
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, U32 value) {
		return v8convert<v8::Local<v8::Integer>, v8::Local<v8::String>>(isolate, v8::Integer::New(isolate, value));
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, F32 value) {
		return v8convert<v8::Local<v8::Number>, v8::Local<v8::String>>(isolate, v8::Number::New(isolate, value));
	}

	template<>
	inline v8::Local<v8::String> v8convert(v8::Isolate *isolate, v8::Local<v8::Value> value) {
		return value->ToString(isolate);
	}

	inline void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
		v8::HandleScope handle_scope(isolate);
		v8::Local<v8::String> exception = try_catch->Exception()->ToString(isolate);
		std::string exception_string = v8convert<v8::Local<v8::String>, std::string>(exception);
		v8::Local<v8::Message> message = try_catch->Message();
		if (message.IsEmpty()) {
			// V8 didn't provide any extra information about this error; just
			// print the exception.
			fprintf(stderr, "%s\n", exception_string.c_str());
		} else {
			// Print (filename):(line number): (message).
			v8::Local<v8::String> filename = message->GetScriptOrigin().ResourceName()->ToString(isolate);
			v8::Local<v8::Context> context(isolate->GetCurrentContext());
			std::string filename_string = v8convert<v8::Local<v8::String>, std::string>(filename);
			int linenum = message->GetLineNumber(context).FromJust();
			fprintf(stderr, "%s:%i: %s\n", filename_string.c_str(), linenum, exception_string.c_str());
			// Print line of source code.
			v8::Local<v8::String> sourceline = message->GetSourceLine(context).ToLocalChecked()->ToString(isolate);
			std::string sourceline_string = v8convert<v8::Local<v8::String>, std::string>(sourceline);
			fprintf(stderr, "%s\n", sourceline_string.c_str());
			// Print wavy underline (GetUnderline is deprecated).
			int start = message->GetStartColumn(context).FromJust();
			for (int i = 0; i < start; i++) {
				fprintf(stderr, " ");
			}
			int end = message->GetEndColumn(context).FromJust();
			for (int i = start; i < end; i++) {
				fprintf(stderr, "^");
			}
			fprintf(stderr, "\n");
			v8::Local<v8::String> stack_trace = try_catch->StackTrace(context).ToLocalChecked()->ToString(isolate);
			if (stack_trace->Length() > 0) {
				std::string stack_trace_string = v8convert<v8::Local<v8::String>, std::string>(stack_trace);
				fprintf(stderr, "%s\n", stack_trace_string.c_str());
			}
		}
	}

	inline v8::Local<v8::Array> createStringArray(v8::Isolate *isolate, int count, const char **items) {
		v8::EscapableHandleScope scope(isolate);

		v8::Local<v8::Array> array = v8::Array::New(isolate);
		for (U32 i = 0; i < count; i ++) {
			v8::Local<v8::String> str = V8Utils::v8convert<const char *, v8::Local<v8::String>>(isolate, items[i]);
			array->Set(i, str);
		}

		return scope.Escape(array);
	}

	template<typename T>
	inline v8::Local<v8::Array> createArray(v8::Isolate *isolate, const std::vector<T> &objects) {
		v8::EscapableHandleScope scope(isolate);

		v8::Local<v8::Array> array = v8::Array::New(isolate);
		for (U32 i = 0; i < objects.size(); i ++) {
			v8::Local<v8::String> str = V8Utils::v8convert<T, v8::Local<v8::String>>(isolate, objects[i]);
			array->Set(i, str);
		}

		return scope.Escape(array);
	}

	template<typename T>
	inline void concat_array(v8::Isolate *isolate, std::vector<v8::Local<v8::Value>> &array, T first) {
		array.push_back(v8convert<T, v8::Local<v8::String>>(isolate, first));
	}
	template<typename T, typename... Args>
	inline void concat_array(v8::Isolate *isolate, std::vector<v8::Local<v8::Value>> &array, T first, Args... args) {
		concat_array(isolate, array, first);
		concat_array(isolate, array, args...);
	}
	template<typename... Args>
	inline v8::Local<v8::Array> createArray(v8::Isolate *isolate, Args... args) {
		v8::EscapableHandleScope scope(isolate);

		std::vector<v8::Local<v8::Value>> array;
		concat_array(isolate, array, args...);

		return scope.Escape(createArray(isolate, array));

	}
};

#endif
