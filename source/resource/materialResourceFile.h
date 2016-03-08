//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_MATERIALRESOURCEFILE_H_
#define _RESOURCE_MATERIALRESOURCEFILE_H_

#include "resource/IResource.h"

/**
 * This class just caches a materials file to show that it has already been
 * loaded on the disk. Nothing more, nothing less.
 */
class MaterialResourceFile : public IResource {
public:
	MaterialResourceFile(const std::string &file) {
		mFile = file;
	}

	inline const std::string getFile() const {
		return mFile;
	}
private:
	std::string mFile;
};

#endif // _RESOURCE_MATERIALRESOURCEFILE_H_
