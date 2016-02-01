//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef renderInfo_h
#define renderInfo_h

#define SRUTIL_DELEGATE_PREFERRED_SYNTAX

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <delegate/delegate.hpp>

#include "platform/platformGL.h"
#include "render/shader.h"
#include "render/material.h"
#include "render/util.h"

using namespace srutil;

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
	glm::vec2 windowSize;
	F32 pixelDensity;

#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
	typedef delegate<void(Material *, RenderInfo &, void *)> RenderMethod;
#else
	typedef delegate<void, Material *, RenderInfo &> RenderMethod;
#endif
	std::unordered_map<Material *, std::vector<std::pair<RenderMethod, void *>>> renderMethods;

	void loadShader(Shader *shader) const {
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

	/**
	 * Add a method that will be rendered when the renderInfo is submitted. These
	 * render methods are grouped by material.
	 * @param material The material that this render method uses.
	 * @param method   A function (or lambda expression) that will be called to
	 *                 perform the rendering.
	 */
	inline void addRenderMethod(Material *material, RenderMethod method, void *userinfo = nullptr) {
		renderMethods[material].push_back(std::make_pair(method, userinfo));
	}

	/**
	 * Clear the current list of render methods
	 */
	inline void clearRenderMethods() {
		renderMethods.clear();
	}

	/**
	 * Submit all stored render methods in the renderInfo.
	 */
	void render() {
		//Render methods are sorted by material
		for (auto &pair : renderMethods) {
			//Load the material for the methods
			Material *material = pair.first;
			material->activate();

			if (material->getShader() == nullptr)
				material->setShader(Shader::defaultShader);

			loadShader(material->getShader());

			//And call every method that uses this material
			for (auto &methodPair : pair.second) {
				methodPair.first(material, *this, methodPair.second);

				GL_CHECKERRORS();
			}

			//Deactivate the material before loading the next one
			material->deactivate();
		}
	}
};

#endif 
