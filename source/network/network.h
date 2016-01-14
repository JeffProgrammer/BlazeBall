//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_NETWORK_H_
#define _PLATFORM_NETWORK_H_

#include <enetpp/global_state.h>

namespace Network {
	inline void init() {
#ifndef EMSCRIPTEN
		enetpp::global_state::get().initialize();
#endif
	}

	inline const bool isInitialized() {
#ifndef EMSCRIPTEN
		return enetpp::global_state::get().is_initialized();
#else
		return true;
#endif
	}

	inline void destroy() {
#ifndef EMSCRIPTEN
		enetpp::global_state::get().deinitialize();
#endif
	}
}

#endif
