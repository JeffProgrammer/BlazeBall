//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_MESHRESOURCE_H_
#define _RESOURCE_MESHRESOURCE_H_

#include "resource/IResource.h"

class MeshResource : public IResource {
	typedef IResource Parent;
public:
	MeshResource(const std::string &file);
	~MeshResource();

	virtual bool load() = 0;

protected:
	std::string mResourceFile;
};

#endif // _RESOURCE_MESHRESOURCE_H_
