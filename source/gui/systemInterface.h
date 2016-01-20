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

#ifndef _GUI_SYSTEMINTERFACE_H_
#define _GUI_SYSTEMINTERFACE_H_

#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Input.h>
#include "platform/platformEvent.h"
#include "platform/platformTimer.h"

class GuiInterface : public Rocket::Core::SystemInterface {
private:
	PlatformTimer *mTimer;
public:
	GuiInterface();
	~GuiInterface();

	/**
	 * Translates a key from this engine to Rocket's internal engine.
	 * @param key The key we are translating.
	 * @return the Translated key.
	 */
	static Rocket::Core::Input::KeyIdentifier translateKey(KeyEvent::Key key);

	static S32 translateMouseButton(U8 button);

	virtual F32 GetElapsedTime() override;
	virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String &message) override;
};

#endif // _GUI_SYSTEMINTERFACE_H_