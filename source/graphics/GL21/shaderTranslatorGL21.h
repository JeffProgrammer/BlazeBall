//------------------------------------------------------------------------------
// Copyright (c) 2016, Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of glslTranslator nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_GL21_SHADERTRANSLATORGL21_H_
#define _GRAPHICS_GL21_SHADERTRANSLATORGL21_H_

#include "graphics/shaderTranslator.h"

class ShaderTranslatorGL21 : public ShaderTranslator {
public:
	ShaderTranslatorGL21() {}

	/**
	 * Translates the shader in it's tokenized list form from the base to GLSL 120.
	 * @param str The stream of shader source to be tokenized and translated..
	 * @param shaderType The type of shader stream that is being parsed, such as a
	 *  vertex shader or a fragment (pixel) shader.
	 * @return the translated shader source, back in a string form.
	 */
	virtual const std::string translate(const std::string &str, ShaderType shaderType) override;
};

#endif // _GRAPHICS_GL21_SHADERTRANSLATORGL21_H_