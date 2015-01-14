//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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

#include "shader.h"
#include "io.h"
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

Shader::Shader(String vertPath, String fragPath) {
	programId = loadProgram(vertPath, fragPath);
}

GLuint Shader::loadShader(String path, GLenum type) {
	GLuint shaderId = glCreateShader(type);
	U32 length;
	U8 *data = io->readFile(path, &length);
	if (data == NULL)
		return 0;

	GLint result = GL_FALSE;
	S32 infoLogLength;

	glShaderSource(shaderId, 1, (GLchar **)&data, NULL);
	glCompileShader(shaderId);
	delete data;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	String log = String(infoLogLength);
	glGetShaderInfoLog(shaderId, infoLogLength, NULL, (GLchar *)log.data);

	if (!result) {
		printf("%s error: %s\n", path.data, log.data);
		return 0;
	}

	return shaderId;
}

GLuint Shader::loadProgram(String vertPath, String fragPath) {
	vertId = loadShader(vertPath, GL_VERTEX_SHADER);
	fragId = loadShader(fragPath, GL_FRAGMENT_SHADER);

	if (glGetError())
		return 0;

	GLuint progID = glCreateProgram();
	glAttachShader(progID, vertId);
	glAttachShader(progID, fragId);
	glLinkProgram(progID);

	GLint result = GL_FALSE;
	S32 infoLogLength;

	glGetProgramiv(progID, GL_LINK_STATUS, &result);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);

	String log = String(infoLogLength);
	glGetProgramInfoLog(progID, infoLogLength, NULL, (GLchar *)log.data);

	if (!result) {
		printf("%s\n", log.data);
	}

	glDeleteShader(vertId);
	glDeleteShader(fragId);

	return progID;
}

GLuint Shader::getProgramId() {
	return programId;
}

void Shader::setUniformLocation(String name, GLuint location) {
	GLuint glLocation = glGetUniformLocation(getProgramId(), (const char *)name.data);
	glUniform1i(glLocation, location);
}