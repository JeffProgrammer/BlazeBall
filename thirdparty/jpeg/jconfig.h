/*
 * This file has been modified, to include platform-specific headers.
 * This goes along with the jpeg version inside of dif-viewer.
 *
 * Copyright Jeff Hutchinson and Glenn Smith
 * Same license as libjpeg
 */

#ifdef WIN32
#include "jconfig_msvc.h"
#elif __APPLE__
#include "jconfig_mac.h"
#elif EMSCRIPTEN
#include "jconfig_mac.h"
#else
#error "Please define a configuration file for your platform
#endif