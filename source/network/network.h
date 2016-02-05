//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_NETWORK_H_
#define _PLATFORM_NETWORK_H_

#include <enetpp/global_state.h>

namespace Network {
	inline void init() {
		enetpp::global_state::get().initialize();
	}

	inline const bool isInitialized() {
		return enetpp::global_state::get().is_initialized();
	}

	inline void destroy() {
		enetpp::global_state::get().deinitialize();
	}
}

#endif
