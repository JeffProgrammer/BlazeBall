//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_IRESOURCE_H_
#define _RESOURCE_IRESOURCE_H_

#include "base/types.h"
#include "game/renderedObject.h"

class IResource {
public:
	void attachSceneObject(RenderedObject *object) {
		// Make sure that we do not add this object twice!
		if (std::find(mSceneObjectList.begin(), mSceneObjectList.end(), object) != mSceneObjectList.end()) {
			IO::printf("%p is already bound to the resource %p.\n", object, this);
			return;
		}
		
		mSceneObjectList.push_back(object);
	}

	void removeSceneObject(RenderedObject *object) {
		// check to make sure that we have the object in the list before removing it.
		auto pos = std::find(mSceneObjectList.begin(), mSceneObjectList.end(), object);
		if (pos == mSceneObjectList.end()) {
			IO::printf("%p is not bound to the resource %p.\n", object, this);
			return;
		}
		
		mSceneObjectList.erase(pos);
	}

protected:
	std::vector<RenderedObject*> mSceneObjectList;
};

#endif // _RESOURCE_IRESOURCE_H_