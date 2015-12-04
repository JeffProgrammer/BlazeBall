//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef shader_h
#define shader_h

#include <stdio.h>
#include "base/types.h"

#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl.h>
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

#endif