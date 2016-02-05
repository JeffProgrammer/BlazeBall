//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "platform/platformTimer.h"

// Initialize the global mutex for the timer subsystem.
std::mutex PlatformTimer::sMutex;