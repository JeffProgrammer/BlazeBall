//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
// 
// Some code taken from the SDL2 sample in LibRocket.
//
// Copyright (c) 2008-2010 Nuno Silva
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//------------------------------------------------------------------------------

#include <glm/gtc/matrix_transform.hpp>
#include "gui/renderInterface.h"
#include "render/texture/bitmapTexture.h"
#include "base/io.h"
#include "platform/platformGL.h"
#include "render/util.h"
#include "network/client.h"
#include "render/renderer.h"

GuiRenderInterface::GuiRenderInterface(Client *client, PlatformWindow *window) : mClient(client), mWindow(window) {
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	// Gui shader
	mShader = new Shader("Gui", "guiV.glsl", "guiF.glsl");
	GL_CHECKERRORS();

	mShader->addAttribute("vertexPosition", 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), (void *)offsetof(Rocket::Core::Vertex, position));
	mShader->addAttribute("vertexUV", 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), (void *)offsetof(Rocket::Core::Vertex, tex_coord));
	mShader->addAttribute("vertexColor", 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Rocket::Core::Vertex), (void *)offsetof(Rocket::Core::Vertex, colour));
	mShader->addUniformLocation("textureSampler", 0);
}

GuiRenderInterface::~GuiRenderInterface() {
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);
	delete mShader;
}

// TODO: have a pool of vertex buffers to stream draw. Using one is very slow
//       and could cause sync points in the driver
// TODO: implement buffer orphaning techniques to potentially trick the driver
//       to use a ring buffer in constant allocations of vram.
void GuiRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation) {
	glDisable(GL_DEPTH_TEST);
	mShader->activate();
	GL_CHECKERRORS();

	// upload vertex data to the gl
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Rocket::Core::Vertex) * num_vertices, &vertices[0], GL_STREAM_DRAW);
	GL_CHECKERRORS();


	// upload indices to the GL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, &indices[0], GL_STREAM_DRAW);
	GL_CHECKERRORS();

	// Bind shader
	mShader->enableAttributes();
	GL_CHECKERRORS();

	auto glTexture = reinterpret_cast<BitmapTexture*>(texture);
	if (glTexture != nullptr) {
		glTexture->activate(GL_TEXTURE0);
		GL_CHECKERRORS();
	}
	GL_CHECKERRORS();
	mShader->setUniform<GLint>("hasTexture", (glTexture != NULL) ? 1 : 0);
	GL_CHECKERRORS();
	auto vec = glm::vec2(translation.x, translation.y);
	mShader->setUniformVector<glm::vec2>("translation", vec);
	GL_CHECKERRORS();
	//Should be the whole screen
	F32 pixelDensity = glGetPixelDensityEXT();
	glViewport(0, 0, GLsizei(mWindow->getWindowSize().x * pixelDensity), GLsizei(mWindow->getWindowSize().y * pixelDensity));
	glm::mat4 ortho = glm::ortho(0.0f, (float)mWindow->getWindowSize().x * pixelDensity, (float)mWindow->getWindowSize().y * pixelDensity, 0.0f, 0.0f, 1.0f);
	ortho = glm::scale(ortho, glm::vec3(pixelDensity));
	ortho = glm::translate(ortho, glm::vec3(translation.x, translation.y, 0.0f));
	mShader->setUniformMatrix<glm::mat4>("projectionMatrix", GL_FALSE, ortho);
	GL_CHECKERRORS();

	// Draw
	// Notice: We use unsigned ints here. They are potentially slower, so we need to check if we 
	// can convert these to unsigned shorts if we hit a performance issue in our profiling.
	// Also note that if we ever use OpenGLES, it does not support unsigned int in draw calls.
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	GL_CHECKERRORS();

	// unbind data
	mShader->disableAttributes();
	mShader->deactivate();
	GL_CHECKERRORS();
	if (glTexture != nullptr) {
		glTexture->deactivate();
		GL_CHECKERRORS();
	}
	GL_CHECKERRORS();
	glEnable(GL_DEPTH_TEST);
}

void GuiRenderInterface::EnableScissorRegion(bool enabled) {
	if (enabled)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void GuiRenderInterface::SetScissorRegion(S32 x, S32 y, S32 width, S32 height) {
	const auto &size = mWindow->getWindowSize();
	//Need to scissor by pixel density because scaling
	F32 pixelDensity = glGetPixelDensityEXT();
	glScissor(GLint(x * pixelDensity), GLint((size.y - (y + height)) * pixelDensity), GLsizei(width * pixelDensity), GLsizei(height * pixelDensity));
}

bool GuiRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source) {
	auto texture = static_cast<BitmapTexture*>(IO::loadTexture(source.CString()));
	if (texture == nullptr) {
		texture_handle = NULL;
		return false;
	}
	texture->generateBuffer();
	texture_dimensions.x = texture->extent.x;
	texture_dimensions.y = texture->extent.y;
	texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
	return true;
}

bool GuiRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions) {
	auto texture = new BitmapTexture(const_cast<U8*>(reinterpret_cast<const U8*>(source)), glm::ivec2(source_dimensions.x, source_dimensions.y), BitmapTexture::Format::FormatRGBA8);
	if (texture == nullptr) {
		texture_handle = NULL;
		return false;
	}
	texture->generateBuffer();
	texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
	return true;
}

void GuiRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle textureHandle) {
	delete reinterpret_cast<BitmapTexture*>(textureHandle);
}