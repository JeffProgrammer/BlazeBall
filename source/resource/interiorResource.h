//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_INTERIORRESOURCE_H_
#define _RESOURCE_INTERIORRESOURCE_H_

#include <dif/objects/dif.h>
#include <vector>
#include "base/types.h"
#include "base/io.h"
#include "resource/MeshResource.h"

class InteriorResource : public MeshResource {
	typedef MeshResource Parent;
public:
	InteriorResource(const std::string &file);
	~InteriorResource();

	virtual bool load() override;

private:

	/**
	 * Holds the data for the interior.
	 * Material*        - Each material has a number of triangles.
	 * vector<Triangle> - Triangle list for each material.
	 */
	std::unordered_map<Material*, std::vector<Triangle>> triangles;
};

#endif // _RESOURCE_INTERIORRESOURCE_H_
