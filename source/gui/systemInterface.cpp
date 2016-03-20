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

#include "gui/systemInterface.h"
#include "platform/platform.h"
#include "platform/platformEvent.h"
#include "main/gameState.h"

GuiInterface::GuiInterface() {
	mTimer = GameState::gState->platform->createTimer();
	mTimer->start();
}

GuiInterface::~GuiInterface() {
	delete mTimer;
}

Rocket::Core::Input::KeyIdentifier GuiInterface::translateKey(KeyEvent::Key key) {
	return Rocket::Core::Input::KeyIdentifier::KI_0; // for now
}

S32 GuiInterface::translateMouseButton(U8 button) {
	switch (button) {
		case MouseButton::MOUSE_BUTTON_LEFT:
			return 0;
		case MouseButton::MOUSE_BUTTON_RIGHT:
			return 1;
		case MouseButton::MOUSE_BUTTON_MIDDLE:
			return 2;
		default:
			return 3;
	}
}

F32 GuiInterface::GetElapsedTime() {
	return F32(mTimer->getElapsedTimeSinceStart());
}

bool GuiInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String &message) {
	std::string Type;

	switch (type)
	{
	case Rocket::Core::Log::LT_ALWAYS:
		Type = "[Always]";
		break;
	case Rocket::Core::Log::LT_ERROR:
		Type = "[Error]";
		break;
	case Rocket::Core::Log::LT_ASSERT:
		Type = "[Assert]";
		break;
	case Rocket::Core::Log::LT_WARNING:
		Type = "[Warning]";
		break;
	case Rocket::Core::Log::LT_INFO:
		Type = "[Info]";
		break;
	case Rocket::Core::Log::LT_DEBUG:
		Type = "[Debug]";
		break;
	case Rocket::Core::Log::LT_MAX:
		break;
	};

	IO::printf("%s - %s\n", Type.c_str(), message.CString());

	return true;
}