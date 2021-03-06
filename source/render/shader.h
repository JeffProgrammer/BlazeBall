//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#ifndef _RENDER_SHADER_H_
#define _RENDER_SHADER_H_

#include <stdio.h>
#include <string>
#include <unordered_map>
#include "base/types.h"
#include "base/math.h"
#include "platform/platformGL.h"
#include "game/scriptObject.h"

class Shader {
protected:
	struct AttributeInfo {
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid *pointer;
	};

	GLuint mVertId;
	GLuint mFragId;
	GLuint mProgramId;

	std::unordered_map<std::string, GLuint> mUniformLocations;
	std::unordered_map<std::string, GLuint> mAttribLocations;
	std::unordered_map<std::string, GLuint> mSetUniformLocations;
	std::unordered_map<std::string, AttributeInfo> mAttributes;

	/**
	 * The name of the shader object.
	 * Note that this name will be used in script to determine what
	 * shader to attach to objects and/or materials.
	 */
	std::string mName;

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

	static std::unordered_map<std::string, Shader*> sShaderTable;
public:
	static Shader *defaultShader;

	/**
	 * Construct a shader from given vertex and fragment shader files.
	 * @param vertPath The path to the file containing the vertex shader
	 * @param fragPath The path to the file containing the fragment shader
	 */
	Shader(const std::string &name, const std::string &vertPath, const std::string &fragPath);
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
	GLint getUniformLocation(const std::string &name);
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

	/**
	 * Add a uniform location that will be set when the shader is activated
	 * @param name     The name of the uniform
	 * @param position The position where its location will be set
	 */
	void addUniformLocation(const std::string &name, GLuint position);

	/**
	 * Enable a vertex attribute array in OpenGL
	 * @param name       The name of the attribute to enable
	 * @param size       The number of components per vertex attribute
	 * @param type       The data type of the components in the array
	 * @param normalized If the data are normalized
	 * @param stride     The offset from one vertex to the next in the data
	 * @param pointer    A pointer to the offset of the first component of the first vertex
	 */
	void enableAttribute(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride = 0, const GLvoid *pointer = 0);
	/**
	 * Disable a vertex attribute array in OpenGL
	 * @param name The name of the attribute to disable
	 */
	void disableAttribute(const std::string &name);

	/**
	 * Store the information of an attribute to be enabled with calls to enableAttributes()
	 * @param name       The name of the attribute to store
	 * @param size       The number of components per vertex attribute
	 * @param type       The data type of the components in the array
	 * @param normalized If the data are normalized
	 * @param stride     The offset from one vertex to the next in the data
	 * @param pointer    A pointer to the offset of the first component of the first vertex
	 */
	void addAttribute(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride = 0, const GLvoid *pointer = 0);
	/**
	 * Enable all saved attributes that were added using addAttribute()
	 */
	void enableAttributes();
	/**
	 * Disable all saved attributes that were added using addAttribute()
	 */
	void disableAttributes();

	/**
	 * Set a scalar uniform's value for the shader
	 * @param name The name of the uniform to set
	 * @param data The desired value
	 */
	template <typename T>
	void setUniform(const std::string &name, const T &data);

	/**
	 * Set a vector uniform's value for the shader
	 * @param name The name of the uniform to set
	 * @param data The desired value
	 */
	template <typename T>
	void setUniformVector(const std::string &name, const T &data);

	/**
	 * Set a matrix uniform's value for the shader
	 * @param name      The name of the uniform to set
	 * @param transpose If the matrix should be transposed
	 * @param data      The desired value
	 */
	template <typename T>
	void setUniformMatrix(const std::string &name, GLboolean transpose, const T &data);

	/**
	 * Gets the name of the shader object.
	 * @return the name of the shader object.
	 */
	const std::string getName() const {
		return mName;
	}

	/**
	 * Searches the shader hash table to find a shader object specified by the 
	 * name.
	 * @param name The shader who's mName property reflects the desired search.
	 * @return The Shader object, or nullptr if it is not found.
	 */
	static Shader* getShaderByName(const std::string &name) {
		// ensure we have it.
		if (sShaderTable.find(name) == sShaderTable.end())
			return nullptr;
		return sShaderTable[name];
	}

	/**
	 * Deletes all of the shaders within the hash table.
	 */
	static void destroyAllShaders() {
		// TODO:
		//for (auto shader : sShaderTable)
			//delete shader.second;
	}

	/**
	 * Initializes all of the shaders that are to be used within the game.
	 */
	static void initializeShaders();
};

#define ShaderUniformScalar(type, fn) \
template<> inline void Shader::setUniform(const std::string &name, const type &data) { \
	GLint location = getUniformLocation(name); \
	if (location == -1) { \
		return; \
	} \
	fn(location, data); \
}

ShaderUniformScalar(GLint, glUniform1i);
ShaderUniformScalar(GLfloat, glUniform1f);

#define ShaderUniformVector(type, fn) \
template<> inline void Shader::setUniformVector(const std::string &name, const type &data) { \
	GLint location = getUniformLocation(name); \
	if (location == -1) { \
		return; \
	} \
	fn(location, 1, &data[0]); \
}

ShaderUniformVector(glm::vec2, glUniform2fv);
ShaderUniformVector(glm::vec3, glUniform3fv);
ShaderUniformVector(glm::vec4, glUniform4fv);

ShaderUniformVector(glm::ivec2, glUniform2iv);
ShaderUniformVector(glm::ivec3, glUniform3iv);
ShaderUniformVector(glm::ivec4, glUniform4iv);

#define ShaderUniformMatrix(type, fn) \
template<> inline void Shader::setUniformMatrix(const std::string &name, GLboolean transpose, const type &data) { \
	GLint location = getUniformLocation(name); \
	if (location == -1) { \
		return; \
	} \
	fn(location, 1, transpose, &data[0][0]); \
}

ShaderUniformMatrix(glm::mat3, glUniformMatrix3fv);
ShaderUniformMatrix(glm::mat4, glUniformMatrix4fv);

#endif // _RENDER_SHADER_H_