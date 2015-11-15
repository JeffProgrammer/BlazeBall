//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// return type implementation
//-----------------------------------------------------------------------------

template<> inline U8 ScriptEngine::getReturnValue(asIScriptContext *context) {
	return static_cast<U8>(context->GetReturnByte());
}
template<> inline U16 ScriptEngine::getReturnValue(asIScriptContext *context) {
	return static_cast<U16>(context->GetReturnWord());
}
template<> inline U32 ScriptEngine::getReturnValue(asIScriptContext *context) {
	return static_cast<U32>(context->GetReturnDWord());
}
template<> inline U64 ScriptEngine::getReturnValue(asIScriptContext *context) {
	return static_cast<U64>(context->GetReturnQWord());
}
template<> inline F32 ScriptEngine::getReturnValue(asIScriptContext *context) {
	return static_cast<F32>(context->GetReturnFloat());
}
template<> inline F64 ScriptEngine::getReturnValue(asIScriptContext *context) {
	return static_cast<F64>(context->GetReturnDouble());
}

//-----------------------------------------------------------------------------
// Execution functions for executing the script.
//-----------------------------------------------------------------------------

template<typename T> inline T ScriptEngine::executeFunction() {
	asIScriptContext *context = mCurrentContext;
	S32 code = context->Execute();
	if (code != asEXECUTION_FINISHED) {
		if (code == asEXECUTION_EXCEPTION) {
			// we got an exception
			printf("Exception: %s\n", context->GetExceptionString());
			printf("Function: %s\n", context->GetExceptionFunction()->GetDeclaration());
			printf("Line: %d\n", context->GetExceptionLineNumber());
		}
	}

	// get return type
	T ret = getReturnValue<T>(context);
	context->Unprepare();
	return ret;
}

template<> inline void ScriptEngine::executeFunction() {
	asIScriptContext *context = mCurrentContext;
	S32 code = context->Execute();
	if (code != asEXECUTION_FINISHED) {
		if (code == asEXECUTION_EXCEPTION) {
			// we got an exception
			printf("Exception: %s\n", context->GetExceptionString());
			printf("Function: %s\n", context->GetExceptionFunction()->GetDeclaration());
			printf("Line: %d\n", context->GetExceptionLineNumber());
		}
	}
	context->Unprepare();
}