//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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

#include "gui/renderInterface.h"
#include "render/texture/bitmapTexture.h"
#include "base/io.h"
#include "platform/platformGL.h"

GuiRenderInterface::GuiRenderInterface(PlatformWindow *window) {
	mWindow = window;
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	// Gui shader
	mShader = new Shader("Gui", "guiV.glsl", "guiF.glsl");
	mShader->addAttribute("vertexPosition", 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vector2f));
	mShader->addAttribute("vertexUV", 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vector2f));
	mShader->addAttribute("vertexColor", 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Rocket::Core::Colourb));
	mShader->addUniformLocation("diffuseTexture", 0);
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
	// upload vertex data to the gl
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Rocket::Core::Vertex) * num_vertices, &vertices[0], GL_STREAM_DRAW);

	// upload indices to the GL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, &indices[0], GL_STREAM_DRAW);

	// Bind shader
	mShader->enableAttributes();
	mShader->setUniform<GLint>("hasTexture", (texture != NULL) ? 1 : 0);
	mShader->activate();

	BitmapTexture *glTexture = NULL;
	if (texture != NULL) {
		auto glTexture = reinterpret_cast<BitmapTexture*>(texture);
		glTexture->activate(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
	}

	// Draw
	// Notice: We use unsigned ints here. They are potentially slower, so we need to check if we 
	// can convert these to unsigned shorts if we hit a performance issue in our profiling.
	// Also note that if we ever use OpenGLES, it does not support unsigned int in draw calls.
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	// unbind data
	mShader->disableAttributes();
	mShader->deactivate();
	if (texture != NULL) {
		glTexture->deactivate();
		glEnable(GL_TEXTURE_2D);
	}
}

void GuiRenderInterface::EnableScissorRegion(bool enabled) {
	if (enabled)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void GuiRenderInterface::SetScissorRegion(S32 x, S32 y, S32 width, S32 height) {
	const auto &size = mWindow->getWindowSize();
	glScissor(x, size.x - (y + height), width, height);
}

bool GuiRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source) {
	auto texture = static_cast<BitmapTexture*>(IO::loadTexture(source.CString()));
	if (texture == nullptr)
		return false;
	texture->generateBuffer();
	texture_dimensions.x = texture->extent.x;
	texture_dimensions.y = texture->extent.y;
	texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
	return true;
}

bool GuiRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions) {
	auto texture = new BitmapTexture(const_cast<U8*>(reinterpret_cast<const U8*>(source)), glm::ivec2(source_dimensions.x, source_dimensions.y), BitmapTexture::Format::FormatRGBA8);
	if (texture == nullptr)
		return false;
	texture->generateBuffer();
	texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
	return true;
}

void GuiRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle textureHandle) {
	delete reinterpret_cast<BitmapTexture*>(textureHandle);
}