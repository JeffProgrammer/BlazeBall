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

//-----------------------------------------------------------------------------
// Parameters for executing functions.
//-----------------------------------------------------------------------------

template<> inline void ScriptEngine::setParameter(S32 index, U64 arg) {
	mCurrentContext->SetArgQWord(index, static_cast<asQWORD>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, S64 arg) {
	mCurrentContext->SetArgQWord(index, static_cast<asQWORD>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, U32 arg) {
	mCurrentContext->SetArgDWord(index, static_cast<asDWORD>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, S32 arg) {
	mCurrentContext->SetArgDWord(index, static_cast<asDWORD>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, U16 arg) {
	mCurrentContext->SetArgWord(index, static_cast<asWORD>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, S16 arg) {
	mCurrentContext->SetArgWord(index, static_cast<asWORD>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, U8 arg) {
	mCurrentContext->SetArgByte(index, static_cast<asBYTE>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, S8 arg) {
	mCurrentContext->SetArgByte(index, static_cast<asBYTE>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, F32 arg) {
	mCurrentContext->SetArgFloat(index, static_cast<float>(arg));
}
template<> inline void ScriptEngine::setParameter(S32 index, F64 arg) {
	mCurrentContext->SetArgDouble(index, static_cast<double>(arg));
}