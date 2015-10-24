//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef renderInfo_h
#define renderInfo_h

#include <glm/glm.hpp>

#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl.h>
#endif

#include "render/shader.h"

//todo: vector also per-shader
#define PROJECTION_MATRIX_UNIFORM_NAME "projectionMat"
#define VIEW_MATRIX_UNIFORM_NAME "viewMat"
#define INVERSE_VIEW_MATRIX_UNIFORM_NAME "inverseViewMat"
#define CAMERA_POSITION_UNIFORM_NAME "cameraPos"

#define LIGHT_COLOR_UNIFORM_NAME "lightColor"
#define AMBIENT_LIGHT_COLOR_UNIFORM_NAME "ambientColor"
#define SUN_POSITION_UNIFORM_NAME "sunPosition"
#define SPECULAR_EXPONENT_UNIFORM_NAME "specularExponent"

struct RenderInfo {
	static glm::mat4 inverseRotMat;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;

	glm::vec4 lightColor;
	glm::vec4 ambientColor;
	glm::vec3 sunPosition;
	U32 specularExponent;

	bool isReflectionPass;

	inline void loadShader(Shader *shader) const {
		if (shader == nullptr)
			shader = Shader::defaultShader;
		
		shader->setUniformMatrix(PROJECTION_MATRIX_UNIFORM_NAME,   GL_FALSE, projectionMatrix);
		shader->setUniformMatrix(VIEW_MATRIX_UNIFORM_NAME,         GL_FALSE, viewMatrix);
		shader->setUniformMatrix(INVERSE_VIEW_MATRIX_UNIFORM_NAME, GL_FALSE, inverseRotMat);
		shader->setUniformVector(CAMERA_POSITION_UNIFORM_NAME,     cameraPosition);
		shader->setUniformVector(LIGHT_COLOR_UNIFORM_NAME,         lightColor);
		shader->setUniformVector(AMBIENT_LIGHT_COLOR_UNIFORM_NAME, ambientColor);
		shader->setUniformVector(SUN_POSITION_UNIFORM_NAME,        sunPosition);
		shader->setUniform      (SPECULAR_EXPONENT_UNIFORM_NAME,   static_cast<F32>(specularExponent));
	}
};

#endif 
