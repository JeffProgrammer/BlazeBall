#set_option(DEPENDENCY_TRACKING "Use gcc -MMD -MT dependency tracking" ON)
#set_option(LIBC                "Use the system C library" ${OPT_DEF_LIBC})
#set_option(GCC_ATOMICS         "Use gcc builtin atomics" ${USE_GCC})
#set_option(ASSEMBLY            "Enable assembly routines" ${OPT_DEF_ASM})
#set_option(SSEMATH             "Allow GCC to use SSE floating point math" ${OPT_DEF_SSEMATH})
#set_option(MMX                 "Use MMX assembly routines" ${OPT_DEF_ASM})
#set_option(3DNOW               "Use 3Dnow! MMX assembly routines" ${OPT_DEF_ASM})
#set_option(SSE                 "Use SSE assembly routines" ${OPT_DEF_ASM})
#set_option(SSE2                "Use SSE2 assembly routines" ${OPT_DEF_SSEMATH})
#set_option(ALTIVEC             "Use Altivec assembly routines" ${OPT_DEF_ASM})
#set_option(DISKAUDIO           "Support the disk writer audio driver" ON)
#set_option(DUMMYAUDIO          "Support the dummy audio driver" ON)
#set_option(VIDEO_DIRECTFB      "Use DirectFB video driver" OFF)
#dep_option(DIRECTFB_SHARED     "Dynamically load directfb support" ON "VIDEO_DIRECTFB" OFF)
#set_option(FUSIONSOUND         "Use FusionSound audio driver" OFF)
#dep_option(FUSIONSOUND_SHARED  "Dynamically load fusionsound audio support" ON "FUSIONSOUND_SHARED" OFF)
#set_option(VIDEO_DUMMY         "Use dummy video driver" ON)
#set_option(VIDEO_OPENGL        "Include OpenGL support" ON)
#set_option(VIDEO_OPENGLES      "Include OpenGL ES support" ON)
#set_option(PTHREADS            "Use POSIX threads for multi-threading" ${UNIX_OR_MAC_SYS})
#dep_option(PTHREADS_SEM        "Use pthread semaphores" ON "PTHREADS" OFF)
#set_option(SDL_DLOPEN          "Use dlopen for shared object loading" ON)
#set_option(OSS                 "Support the OSS audio API" ${UNIX_SYS})
#set_option(ALSA                "Support the ALSA audio API" ${UNIX_SYS})
#dep_option(ALSA_SHARED         "Dynamically load ALSA audio support" ON "ALSA" OFF)
#set_option(ESD                 "Support the Enlightened Sound Daemon" ${UNIX_SYS})
#dep_option(ESD_SHARED          "Dynamically load ESD audio support" ON "ESD" OFF)
#set_option(PULSEAUDIO          "Use PulseAudio" ${UNIX_SYS})
#dep_option(PULSEAUDIO_SHARED   "Dynamically load PulseAudio support" ON "PULSEAUDIO" OFF)
#set_option(ARTS                "Support the Analog Real Time Synthesizer" ${UNIX_SYS})
#dep_option(ARTS_SHARED         "Dynamically load aRts audio support" ON "ARTS" OFF)
#set_option(NAS                 "Support the NAS audio API" ${UNIX_SYS})
#set_option(NAS_SHARED          "Dynamically load NAS audio API" ${UNIX_SYS})
#set_option(SNDIO               "Support the sndio audio API" ${UNIX_SYS})
#set_option(RPATH               "Use an rpath when linking SDL" ${UNIX_SYS})
#set_option(CLOCK_GETTIME       "Use clock_gettime() instead of gettimeofday()" OFF)
#set_option(INPUT_TSLIB         "Use the Touchscreen library for input" ${UNIX_SYS})
#set_option(VIDEO_X11           "Use X11 video driver" ${UNIX_SYS})
#set_option(VIDEO_WAYLAND       "Use Wayland video driver" OFF) #${UNIX_SYS})
#set_option(VIDEO_MIR           "Use Mir video driver" OFF) #${UNIX_SYS})
#dep_option(X11_SHARED          "Dynamically load X11 support" ON "VIDEO_X11" OFF)
#set(SDL_X11_OPTIONS Xcursor Xinerama XInput Xrandr Xscrnsaver XShape Xvm)
#foreach(_SUB ${SDL_X11_OPTIONS})
  #string(TOUPPER "VIDEO_X11_${_SUB}" _OPT)
  #dep_option(${_OPT}           "Enable ${_SUB} support" ON "VIDEO_X11" OFF)
#endforeach()
#set_option(VIDEO_COCOA         "Use Cocoa video driver" ${APPLE})
#set_option(DIRECTX ${WINDOWS})
#set_option(RENDER_D3D          "Enable the Direct3D render driver" ${WINDOWS})

# TODO: We should (should we?) respect cmake's ${BUILD_SHARED_LIBS} flag here
# The options below are for compatibility to configure's default behaviour.
#set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library") # Jeff: Static link pls by default, so OFF
#set(SDL_STATIC ON CACHE BOOL "Build a static version of the library")

#------------------------------------------------------------------------------

set(LIBC ON)
set(GCC_ATOMICS ${USE_GCC})
set(ASSEMBLY ${OPT_DEF_ASM})
set(SSEMATH ${OPT_DEF_SSEMATH})
set(MMX ${OPT_DEF_ASM})
set(3DNOW ${OPT_DEF_ASM})
set(SSE ${OPT_DEF_ASM})
set(SSE2 ${OPT_DEF_SSEMATH})
set(ALTIVEC OFF) # We aren't supporting the PowerPC architecture
set(DISKAUDIO OFF) # using fmod not SDL audio
set(DUMMYAUDIO OFF) # using fmod not SDL audio
set(VIDEO_DIRECTFB OFF)
set(DIRECTFB_SHARED OFF)
set(FUSIONSOUND OFF) # using fmod not SDL audio
set(FUSIONSOUND_SHARED OFF) # using fmod not SDL audio
set(VIDEO_DUMMY ON)
set(VIDEO_OPENGL ON)
set(VIDEO_OPENGLES OFF)
set(PTHREADS ${UNIX_OR_MAC_SYS})
set(PTHREADS_SEM ${UNIX_OR_MAC_SYS})
set(SDL_DLOPEN ON)
set(OSS OFF) # using fmod not SDL audio
set(ALSA OFF) # using fmod not SDL audio
set(ALSA_SHARED OFF) # using fmod not SDL audio
set(ESD OFF) # using fmod not SDL audio
set(ESD_SHARED OFF) # using fmod not SDL audio
set(PULSEAUDIO OFF) # using fmod not SDL audio
set(PULSEAUDIO_SHARED OFF) # using fmod not SDL audio
set(ARTS OFF) # using fmod not SDL audio
set(ARTS_SHARED OFF) # using fmod not SDL audio
set(NAS OFF) # using fmod not SDL audio
set(NAS_SHARED OFF) # using fmod not SDL audio
set(SNDIO OFF) # using fmod not SDL audio
set(RPATH ${UNIX_SYS})
set(CLOCK_GETTIME OFF)
set(INPUT_TSLIB ${UNIX_SYS})
set(VIDEO_X11 ${UNIX_SYS})
set(VIDEO_WAYLAND OFF)
set(VIDEO_MIR OFF)
if (VIDEO_X11)
	set(X11_SHARED ON)
else()
	set(X11_SHARED OFF)
endif()

set(SDL_X11_OPTIONS Xcursor Xinerama XInput Xrandr Xscrnsaver XShape Xvm)
foreach(_SUB ${SDL_X11_OPTIONS})
  string(TOUPPER "VIDEO_X11_${_SUB}" _OPT)
  if (VIDEO_X11)
	set(${_OPT} ON)
  else()
	set(${_OPT} OFF)
  endif()
endforeach()
set(VIDEO_COCOA ${APPLE})
set(DIRECTX ${WINDOWS})
set(RENDER_D3D OFF) # Not using D3D

# We want to static link only pls
set(SDL_SHARED OFF)
set(SDL_STATIC ON)