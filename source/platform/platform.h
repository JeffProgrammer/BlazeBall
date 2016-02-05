//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_PLATFORM_H_
#define _PLATFORM_PLATFORM_H_

#include <memory>
#include "platform/platformTimer.h"
#include "platform/platformWindow.h"
#include "physics/physicsEngine.h"

class Platform {
public:
	struct PhysicsEngineFactory {
		virtual PhysicsEngine *createPhysicsEngine() = 0;
	};

protected:
	std::unique_ptr<PhysicsEngineFactory> mPhysicsFactory;

public:
	Platform(std::unique_ptr<PhysicsEngineFactory> &&physFactory) {
		mPhysicsFactory = std::move(physFactory);
	}
	virtual ~Platform() {}

	virtual PlatformTimer *createTimer() = 0;
	virtual PlatformWindow *createWindow() = 0;
	PhysicsEngine *createPhysicsEngine() {
		return mPhysicsFactory->createPhysicsEngine();
	}

	template<typename T>
	struct ConcretePhysicsEngineFactory : public PhysicsEngineFactory {
		virtual PhysicsEngine *createPhysicsEngine() {
			return new T();
		}
	};
};

#endif
