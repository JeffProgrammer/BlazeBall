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

#ifndef shader_h
#define shader_h

#include <stdio.h>
#include "base/types.h"

#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/OpenGL.h>
#endif

#include <string>
#include <unordered_map>

class Shader {
protected:
	struct AttributeInfo {
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid *pointer;
	};

	GLuint vertId;
	GLuint fragId;
	GLuint programId;

	std::unordered_map<std::string, GLuint> mUniformLocations;
	std::unordered_map<std::string, GLuint> mAttribLocations;
	std::unordered_map<std::string, GLuint> mSetUniformLocations;
	std::unordered_map<std::string, AttributeInfo> mAttributes;

	/**
	 * Load a shader from a given path with a given type.
	 * @param path The file path of the shader to load
	 * @param type The shader's type (fragment/vertex)
	 * @return The OpenGL id for the shader, or 0 if the operation fails.
	 */
	GLuint loadShader(const std::string &path, const GLenum &type);
	/**
	 * Load a program from vertex and fragment shader files
	 * @param vertPath The path to the file containing the vertex shader
	 * @param fragPath The path to the file containing the fragment shader
	 * @return The OpenGL id for the program, or 0 if the operation fails.
	 */
	GLuint loadProgram(const std::string &vertPath, const std::string &fragPath);
public:
	static Shader *defaultShader;

	/**
	 * Construct a shader from given vertex and fragment shader files.
	 * @param vertPath The path to the file containing the vertex shader
	 * @param fragPath The path to the file containing the fragment shader
	 */
	Shader(const std::string &vertPath, const std::string &fragPath);
	/**
	 * Destroy the shader and free its program.
	 */
	~Shader();
	
	/**
	 * Get the OpenGL id of the shader's program, for activating.
	 * @return The shader's program id.
	 */
	GLuint getProgramId();
	
	/**
	 * Get the location of a uniform accessed by the shader
	 * @param name The name of the uniform
	 * @return The uniform's location.
	 */
	GLuint getUniformLocation(const std::string &name);
	/**
	 * Set the location of a uniform for the shader.
	 * @param name The name of the uniform
	 * @param location The desired location for the uniform
	 */
	void setUniformLocation(const std::string &name, const GLuint &location);
	
	/**
	 * Get the location of an attribute in the shader
	 * @param name The name of the attribute
	 * @return The attribute's location
	 */
	GLuint getAttributeLocation(const std::string &name);
	
	/**
	 * Activate and bind the shader.
	 */
	void activate();
	/**
	 * Deactivate and unbind the shader.
	 */
	void deactivate();

	void enableAttribute(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	void disableAttribute(const std::string &name);

	void addUniformLocation(const std::string &name, GLuint position);
	void addAttribute(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
};

#endif