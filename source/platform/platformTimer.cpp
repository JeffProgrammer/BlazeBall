//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "platform/platformTimer.h"

// Initialize the global mutex for the timer subsystem.
std::mutex PlatformTimer::sMutex;