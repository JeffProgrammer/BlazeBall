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

#include "graphics/shader.h"
#include "base/io.h"

#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#endif

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
	//Try to generate the shader
	programId = loadProgram(vertPath, fragPath);
}

Shader::~Shader() {
	//If we have successfully generated, tell OpenGL to delete our program
	if (getProgramId())
		glDeleteProgram(getProgramId());
}

GLuint Shader::loadShader(const std::string &path, const GLenum &type) {
	//Create a new shader into which we will load our code
	GLuint shaderId = glCreateShader(type);
	
	//Read the file's data
	U32 length;
	U8 *data = IO::readFile(path, length);
	if (data == NULL)
		return 0;
	
	GLint result = GL_FALSE;
	S32 infoLogLength;
	
	//Try to compile the shader
	glShaderSource(shaderId, 1, (const GLchar **)&data, NULL);
	glCompileShader(shaderId);
	delete data;
	
	//Check if we had any errors
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	//Get a log of the errors
	GLchar *log = new GLchar[infoLogLength];
	glGetShaderInfoLog(shaderId, infoLogLength, NULL, log);
	
	//Did we actually have an error? If so, terminate here
	if (!result) {
		printf("%s error: %s\n", path.c_str(), log);
		delete[] log;

		//Errored so we can't return an id
		return 0;
	}
	
	delete[] log;
	return shaderId;
}

GLuint Shader::loadProgram(const std::string &vertPath, const std::string &fragPath) {
	//Clear error so we don't confuse it for our own
	while (glGetError() != GL_NO_ERROR);
	
	//Try to load both the vertex and fragment shaders
	vertId = loadShader(vertPath, GL_VERTEX_SHADER);
	fragId = loadShader(fragPath, GL_FRAGMENT_SHADER);
	
	//If either failed, we can't create a program.
	if (vertId == 0 || fragId == 0)
		return 0;
	
	//If there was any error, then let us know.
	GLenum error = glGetError();
	if (error) {
#ifdef _WIN32
		const char *err = "gluErrorString is not implemented on win32. please check the OSX build for shader errors.";
#else
		const char *err = (const char *)gluErrorString(error);
#endif
		printf("Error loading shader: %s (code %d)\n", err, error);
		return 0;
	}
	
	//Try to create a program from the shaders
	GLuint progID = glCreateProgram();
	glAttachShader(progID, vertId);
	glAttachShader(progID, fragId);
	glLinkProgram(progID);
	
	GLint result = GL_FALSE;
	S32 infoLogLength;
	
	//Check the log to see if we succeeded
	glGetProgramiv(progID, GL_LINK_STATUS, &result);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	//Get the log from the info
	GLchar *log = new GLchar[infoLogLength];
	glGetProgramInfoLog(progID, infoLogLength, NULL, log);
	
	//If we didn't create a shader successfully, let us know.
	if (!result) {
		printf("%s\n", log);
		delete[] log;

		//Clean up the shaders
		glDeleteShader(vertId);
		glDeleteShader(fragId);
		
		//Can't return an id if we didn't succeed.
		return 0;
	}
	
	//Clean up the shaders
	glDeleteShader(vertId);
	glDeleteShader(fragId);
	
	delete[] log;
	return progID;
}

GLuint Shader::getProgramId() {
	return programId;
}

GLuint Shader::getUniformLocation(const std::string &name) {
	return glGetUniformLocation(getProgramId(), name.c_str());
}

void Shader::setUniformLocation(const std::string &name, const GLuint &location) {
	GLuint glLocation = glGetUniformLocation(getProgramId(), name.c_str());
	glUniform1i(glLocation, location);
}

GLuint Shader::getAttributeLocation(const std::string &name) {
	return glGetAttribLocation(getProgramId(), name.c_str());
}

void Shader::activate() {
	glUseProgram(this->getProgramId());
}

void Shader::deactivate() {
	glUseProgram(0);
}