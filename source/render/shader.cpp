//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/shader.h"
#include "base/io.h"
#include "render/util.h"
#include "platform/platformGL.h"

std::unordered_map<std::string, Shader*> Shader::sShaderTable;
Shader *Shader::defaultShader = nullptr;

Shader::Shader(const std::string &name, const std::string &vertPath, const std::string &fragPath) {
	mName = name;

	//Try to generate the shader
	mProgramId = loadProgram(vertPath, fragPath);

	// add to the shader hash table
	sShaderTable[name] = this;
}

Shader::~Shader() {
	//If we have successfully generated, tell OpenGL to delete our program
	if (getProgramId())
		glDeleteProgram(getProgramId());

	// remove from the shader hash table
	sShaderTable.erase(mName);
}

GLuint Shader::loadShader(const std::string &path, const GLenum &type) {
	//Create a new shader into which we will load our code
	GLuint shaderId = glCreateShader(type);
	
	//Read the file's data
	U32 length;
	U8 *data = IO::readFile(path, length);
	if (data == NULL)
		return 0;

	auto shaderSource = glTranslateShaderEXT(reinterpret_cast<const char*>(data), type);
	delete[] data;
	auto src = reinterpret_cast<U8*>(const_cast<char*>(shaderSource.c_str()));
	
	GLint result = GL_FALSE;
	S32 infoLogLength;

	//Try to compile the shader
	glShaderSource(shaderId, 1, (const GLchar**)&src, NULL);
	glCompileShader(shaderId);
	delete[] data;
	
	//Check if we had any errors
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	//Get a log of the errors
	GLchar *log = new GLchar[infoLogLength];
	glGetShaderInfoLog(shaderId, infoLogLength, NULL, log);
	
	//Did we actually have an error? If so, terminate here
	if (!result) {
		IO::printf("%s error: %s\n", path.c_str(), log);
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
	mVertId = loadShader(vertPath, GL_VERTEX_SHADER);
	mFragId = loadShader(fragPath, GL_FRAGMENT_SHADER);
	
	//If either failed, we can't create a program.
	if (mVertId == 0 || mFragId == 0)
		return 0;
	
	//If there was any error, then let us know.
	GLenum error = glGetError();
	if (error) {
		IO::printf("Error loading shader: code %d\n", error);
		return 0;
	}
	
	//Try to create a program from the shaders
	GLuint progID = glCreateProgram();
	glAttachShader(progID, mVertId);
	glAttachShader(progID, mFragId);
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
		IO::printf("%s\n", log);
		delete[] log;

		//Clean up the shaders
		glDeleteShader(mVertId);
		glDeleteShader(mFragId);
		
		//Can't return an id if we didn't succeed.
		return 0;
	}
	
	//Clean up the shaders
	glDeleteShader(mVertId);
	glDeleteShader(mFragId);
	
	delete[] log;
	return progID;
}

GLuint Shader::getProgramId() {
	return mProgramId;
}

GLint Shader::getUniformLocation(const std::string &name) {
	// store location in hashmap to avoid gpu sync point everytime we call this method.
	if (mUniformLocations.find(name) == mUniformLocations.end())
		mUniformLocations[name] = glGetUniformLocation(getProgramId(), name.c_str());
	return mUniformLocations[name];
}

void Shader::setUniformLocation(const std::string &name, const GLuint &location) {
	// store location in hashmap to avoid gpu sync point everytime we call this method.
	if (mUniformLocations.find(name) == mUniformLocations.end())
		mUniformLocations[name] = glGetUniformLocation(getProgramId(), name.c_str());
	GLuint uniformLocation = mUniformLocations[name];
	//Make sure we don't try to set an invalid uniform's location
	if (uniformLocation == -1) {
		fprintf(stderr, "Invalid uniform location (-1) for uniform %s\n", name.c_str());
		return;
	}
	glUniform1i(uniformLocation, location);
}

GLuint Shader::getAttributeLocation(const std::string &name) {
	// store location in hashmap to avoid gpu sync point everytime we call this method.
	if (mAttribLocations.find(name) == mAttribLocations.end())
		mAttribLocations[name] = glGetAttribLocation(getProgramId(), name.c_str());
	return mAttribLocations[name];
}

void Shader::activate() {
	glUseProgram(this->getProgramId());
	GL_CHECKERRORS();

	for (auto &setUniform : mSetUniformLocations) {
		setUniformLocation(setUniform.first, setUniform.second);
	}
}

void Shader::deactivate() {

}

void Shader::addUniformLocation(const std::string &name, GLuint position) {
	mSetUniformLocations[name] = position;
}

void Shader::enableAttribute(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	U32 location = getAttributeLocation(name);

	//Don't try to enable an attribute that we don't have
	if (location == -1) {
		fprintf(stderr, "Invalid attribute location (-1) for attribute %s\n", name.c_str());
		return;
	}

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, type, normalized, stride, pointer);
}

void Shader::disableAttribute(const std::string &name) {
	U32 location = getAttributeLocation(name);

	//Don't try to disaable an attribute that we don't have
	if (location == -1) {
		fprintf(stderr, "Invalid attribute location (-1) for attribute %s\n", name.c_str());
		return;
	}

	glDisableVertexAttribArray(location);
}

void Shader::addAttribute(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	AttributeInfo info;
	info.size = size;
	info.type = type;
	info.normalized = normalized;
	info.stride = stride;
	info.pointer = pointer;

	mAttributes[name] = info;
}

void Shader::enableAttributes() {
	for (auto &attribute : mAttributes) {
		enableAttribute(attribute.first, attribute.second.size, attribute.second.type, attribute.second.normalized, attribute.second.stride, attribute.second.pointer);
	}
}

void Shader::disableAttributes() {
	for (auto &attribute : mAttributes) {
		disableAttribute(attribute.first);
	}
}