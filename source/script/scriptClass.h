//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#ifndef _SCRIPT_SCRIPTCLASS_H_
#define _SCRIPT_SCRIPTCLASS_H_

#include "script/scriptTemplates.h"

class ScriptClassConstructor {
public:
	struct Constructor {
		duk_c_function mConstructor;
		S32 mNumArgs;

		Constructor(duk_c_function constructor, S32 argc) : mConstructor(constructor), mNumArgs(argc) {}

		// Don't use
		Constructor() {}
	};

	struct Method {
		std::string mName;
		duk_c_function mFunction;
		S32 mNumArgs;

		Method(const std::string &name, duk_c_function function, S32 argc, ScriptClassConstructor *constructor) : mName(name), mFunction(function), mNumArgs(argc) {
			constructor->addMethod(*this);
		}
	};

	struct Parent {
		Parent(const std::string &parentName, ScriptClassConstructor *parentConstructor) {
			parentConstructor->setParent(parentName);
		}
	};

	struct Field {
		std::string mName;
		duk_c_function mGetterFn;
		duk_c_function mSetterFn;

		Field(const std::string &name, duk_c_function getter, duk_c_function setter, ScriptClassConstructor *constructor) : mName(name), mGetterFn(getter), mSetterFn(setter) {
			constructor->addField(*this);
		}
	};

	ScriptClassConstructor(const std::string &name, const Constructor &constructor) {
		mName = name;
		mConstructor = constructor;
		mParentName = "";

      mNext = sLast;
      sLast = this;
	}

	void addMethod(const Method &method) {
		mMethods.push_back(method);
	}

	void addField(const Field &field) {
		mFields.push_back(field);
	}

	void setParent(const std::string &parent) {
		mParentName = parent;
	}

	const std::string& getName() const {
		return mName;
	}

	const std::string& getParentName() const {
		return mParentName;
	}

	const std::vector<Method>& getMethods() const {
		return mMethods;
	}

	const std::vector<Field>& getFields() const {
		return mFields;
	}

	const Constructor& getConstructor() const {
		return mConstructor;
	}

   static ScriptClassConstructor *sLast;
   ScriptClassConstructor *mNext;
private:
	std::string mName;
	std::string mParentName;
	std::vector<Method> mMethods;
	std::vector<Field> mFields;
	Constructor mConstructor;
};

/**
* Implements a class and exposes it to script.
*/
#define IMPLEMENT_SCRIPT_OBJECT(klass, numArgs)                                                                       \
	ScriptClassConstructor *klass::__scc##klass = new ScriptClassConstructor(#klass, ScriptClassConstructor::Constructor(klass::__constructor##klass, numArgs))

#define IMPLEMENT_SCRIPT_PARENT(klass, parent)                                                                        \
	ScriptClassConstructor::Parent __spp##klass##parent(#parent, klass::__scc##klass)

/**
* The constructor that is run from javascript.
* Put this inside of the public class definition.
*/
#define ScriptConstructor(klass, theConstructor, numArgs)                                                             \
	static ScriptClassConstructor *__scc##klass;                                                                       \
	static duk_ret_t __scriptDestruct##klass(duk_context *context) {                                                   \
		/* delete the object */                                                                                         \
		duk_get_prop_string(context, 0, "___pointer");                                                                  \
		delete static_cast<klass *>(duk_to_pointer(context, -1));                                                       \
		duk_pop(context);                                                                                               \
		return 0;                                                                                                       \
	}                                                                                                                  \
	/* Class constructor, called by javascript engine */                                                               \
	static duk_ret_t __constructor##klass(duk_context *context) {                                                      \
		duk_ret_t check = __checkConstructor(context, #klass, numArgs);                                                 \
		if (check != 0)                                                                                                 \
			return check;                                                                                                \
		/* Handle our logic. */                                                                                         \
		klass *instance = new klass theConstructor;                                                                     \
		__constructorImplementation##klass(context, instance, numArgs);                                                 \
		__finishConstructor(context, __scriptDestruct##klass, instance);                                                \
		return 0;                                                                                                       \
	}                                                                                                                  \
	static void __constructorImplementation##klass(duk_context *context, klass *object, S32 argc)

#define ScriptMethod(klass, name, rettype, numArgs)                                                                                                                     \
	rettype __methodImplementation##name##klass(duk_context *context, klass *object, S32 argc);                        \
	static duk_ret_t __method##name##klass(duk_context *context) {                                                     \
		return ScriptMethodReturn<rettype>::perform<klass>(context, __methodImplementation##name##klass, numArgs);      \
	}                                                                                                                  \
	ScriptClassConstructor::Method mc##klass##name(#name, __method##name##klass, numArgs, klass::__scc##klass);        \
	rettype __methodImplementation##name##klass(duk_context *context, klass *object, S32 argc)

#define ScriptField(klass, type, scriptname, fieldname, duktype)                                                      \
	static duk_ret_t __settermethod##scriptname##klass(duk_context *context) {                                         \
		duk_push_this(context);                                                                                         \
		duk_get_prop_string(context, -1, "___pointer");                                                                 \
		klass *object = static_cast<klass *>(duk_to_pointer(context, -1));                                              \
		duk_pop(context);                                                                                               \
		type x = static_cast<type>(duk_require_##duktype(context, 0));                                                  \
		object->fieldname = x;                                                                                          \
		return 0;                                                                                                       \
	}                                                                                                                  \
	static duk_ret_t __gettermethod##scriptname##klass(duk_context *context) {                                         \
		duk_push_this(context);                                                                                         \
		duk_get_prop_string(context, -1, "___pointer");                                                                 \
		klass *object = static_cast<klass *>(duk_to_pointer(context, -1));                                              \
		duk_pop(context);                                                                                               \
		duk_push_##duktype(context, object->fieldname);                                                                 \
		return 1;                                                                                                       \
	}                                                                                                                  \
	ScriptClassConstructor::Field __fgsc##scriptname##klass(#scriptname, __gettermethod##scriptname##klass, __settermethod##scriptname##klass, klass::__scc##klass)


#endif // _SCRIPT_SCRIPTCLASS_H_