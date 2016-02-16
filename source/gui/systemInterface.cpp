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