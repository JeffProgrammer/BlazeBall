//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "script/scriptEngine.h"

ScriptFunction(add, F32, 2) {
   F32 a = ScriptParam::get<F32>(context, 0);
   F32 b = ScriptParam::get<F32>(context, 1);
   return a + b;
}