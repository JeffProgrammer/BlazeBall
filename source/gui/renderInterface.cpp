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

#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#endif

GuiRenderInterface::GuiRenderInterface(PlatformWindow *window) {
	mWindow = window;
}

void GuiRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation) {

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
	texture_dimensions.x = texture->extent.x;
	texture_dimensions.y = texture->extent.y;
	texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
}

bool GuiRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions) {
	auto texture = new BitmapTexture(const_cast<U8*>(reinterpret_cast<const U8*>(source)), glm::ivec2(source_dimensions.x, source_dimensions.y), BitmapTexture::Format::FormatRGBA8);
	texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
}

void GuiRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle textureHandle) {
	delete reinterpret_cast<BitmapTexture*>(textureHandle);
}