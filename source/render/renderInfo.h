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

#ifndef _RENDER_RENDERINFO_H_
#define _RENDER_RENDERINFO_H_

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
#define UNIFORM_PROJECTION_MATRIX_NAME    "inProjectionMat"
#define UNIFORM_VIEW_MATRIX_NAME          "inViewMat"
#define UNIFORM_MODEL_MATRIX_NAME         "inModelMat"
#define UNIFORM_INVERSE_MODEL_MATRIX_NAME "inInverseModelMat"
#define UNIFORM_INVERSE_VIEW_MATRIX_NAME  "inInverseViewMat"
#define UNIFORM_CAMERA_POSITION_NAME      "inCameraPos"

#define UNIFORM_LIGHT_COLOR_NAME          "inLightColor"
#define UNIFORM_AMBIENT_LIGHT_COLOR_NAME  "inAmbientColor"
#define UNIFORM_SUN_DIRECTION_NAME        "inSunDirection"
#define UNIFORM_SPECULAR_EXPONENT_NAME    "inSpecularExponent"

#define ATTRIBUTE_POSITION_NAME           "inVertexPosition"
#define ATTRIBUTE_UV_NAME                 "inVertexUV"
#define ATTRIBUTE_NORMAL_NAME             "inVertexNormal"
#define ATTRIBUTE_TANGENT_NAME            "inVertexTangent"
#define ATTRIBUTE_BITANGENT_NAME          "inVertexBitangent"
#define ATTRIBUTE_COLOR_NAME              "inVertexColor"

#define SAMPLER_TEXTURE_NAME              "inTextureSampler"
#define SAMPLER_NORMAL_NAME               "inNormalSampler"
#define SAMPLER_SPECULAR_NAME             "inSpecularSampler"
#define SAMPLER_NOISE_NAME                "inNoiseSampler"
#define SAMPLER_CUBEMAP_NAME              "inCubemapSampler"
#define SAMPLER_DEPTH_NAME                "inDepthSampler"

class RenderWorld;
struct RenderInfo {
	static glm::mat4 inverseRotMat;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;

	glm::vec4 lightColor;
	glm::vec4 ambientColor;
	glm::vec3 sunDirection;
	U32 specularExponent;

	bool isReflectionPass;
	struct {
		glm::ivec2 position;
		glm::ivec2 size;
	} viewport;
	
	F32 pixelDensity;

#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
	typedef delegate<void(Material *, RenderInfo &, void *)> RenderMethod;
	typedef delegate<void(RenderInfo &)> RenderWorldMethod;
#else
	typedef delegate<void, Material *, RenderInfo &> RenderMethod;
	typedef delegate<void, RenderInfo &> RenderWorldMethod;
#endif

	enum RenderOrderPosition {
		Skybox,
		Mesh,
		Shape,
		Debug,
		MaxCount
	};

	std::vector<std::unordered_map<Material *, std::vector<std::pair<RenderMethod, void *>>>> renderMethods;
	RenderWorldMethod renderWorld;

	RenderInfo() : projectionMatrix(1), viewMatrix(1), cameraPosition(0), lightColor(0), ambientColor(0), sunDirection(0), specularExponent(1), isReflectionPass(false), viewport{glm::ivec2(0), glm::ivec2(0)}, pixelDensity(1), renderMethods(RenderOrderPosition::MaxCount), renderWorld() {

	}

	RenderInfo(const RenderInfo &other) :
		//Copy every variable from the other info except for its RenderMethods
		projectionMatrix(other.projectionMatrix),
		viewMatrix(other.viewMatrix),
		cameraPosition(other.cameraPosition),
		lightColor(other.lightColor),
		ambientColor(other.ambientColor),
		sunDirection(other.sunDirection),
		specularExponent(other.specularExponent),
		isReflectionPass(other.isReflectionPass),
		viewport(other.viewport),
		pixelDensity(other.pixelDensity),
		renderMethods(RenderOrderPosition::MaxCount), //Make sure to reset this to have nothing in it when we copy
		renderWorld(other.renderWorld) {

	}

	void loadShader(Shader *shader) const {
		if (shader == nullptr)
			shader = Shader::defaultShader;
		
		shader->setUniformMatrix(UNIFORM_PROJECTION_MATRIX_NAME,   GL_FALSE, projectionMatrix);
		shader->setUniformMatrix(UNIFORM_VIEW_MATRIX_NAME,         GL_FALSE, viewMatrix);
		shader->setUniformMatrix(UNIFORM_INVERSE_VIEW_MATRIX_NAME, GL_FALSE, inverseRotMat);
		shader->setUniformVector(UNIFORM_CAMERA_POSITION_NAME,     cameraPosition);
		shader->setUniformVector(UNIFORM_LIGHT_COLOR_NAME,         lightColor);
		shader->setUniformVector(UNIFORM_AMBIENT_LIGHT_COLOR_NAME, ambientColor);
		shader->setUniformVector(UNIFORM_SUN_DIRECTION_NAME,       sunDirection);
		shader->setUniform      (UNIFORM_SPECULAR_EXPONENT_NAME,   static_cast<F32>(specularExponent));
	}

	/**
	 * Add a method that will be rendered when the renderInfo is submitted. These
	 * render methods are grouped by material.
	 * @param material The material that this render method uses.
	 * @param method   A function (or lambda expression) that will be called to
	 *                 perform the rendering.
	 */
	inline void addRenderMethod(Material *material, RenderMethod method, void *userinfo = nullptr, RenderOrderPosition order = RenderOrderPosition::Shape) {
		renderMethods[order][material].push_back(std::make_pair(method, userinfo));
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
		//First, they're sorted by order
		for (auto &list : renderMethods) {
			//Render methods are sorted by material
			for (auto &pair : list) {
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
	}

	/**
	 * Update OpenGL's viewport to only cover this RenderInfo
	 */
	void setViewport() const {
		glViewport(GLint(viewport.position.x * pixelDensity), GLint(viewport.position.y * pixelDensity), GLsizei(viewport.size.x * pixelDensity), GLsizei(viewport.size.y * pixelDensity));
	}

	/**
	 * Update OpenGL's scissor region to only cover this RenderInfo
	 */
	void setScissor() const {
		glScissor(GLint(viewport.position.x * pixelDensity), GLint(viewport.position.y * pixelDensity), GLsizei(viewport.size.x * pixelDensity), GLsizei(viewport.size.y * pixelDensity));
	}
};

#endif  // _RENDER_RENDERINFO_H_