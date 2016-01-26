Epoxy
=====

|  Linux/GCC  |  Windows/MSVC  |  Binaries  |
| :---------: | :------------: | :--------: |
|  [![Linux Status](https://travis-ci.org/Nephatrine/libepoxy.svg?branch=master)](https://travis-ci.org/Nephatrine/libepoxy)  |  [![Windows Build Status](https://ci.appveyor.com/api/projects/status/cbxs5gyryjkr9lei?svg=true)](https://ci.appveyor.com/project/Nephatrine/libepoxy)  |  [![Binaries](https://img.shields.io/badge/bin-MS%20VC12-brightgreen.svg)](https://ci.appveyor.com/project/Nephatrine/libepoxy/build/artifacts)

Epoxy is a library for handling OpenGL function pointer management for you.

It hides the complexity of ```dlopen()```, ```dlsym()```,
```glXGetProcAddress()```, ```eglGetProcAddress()```, etc. from the developer.
They get to read GL specifications and write code using undecorated function
names like ```glCompileShader()```.

Don't forget to check for your extensions or versions being present before you
use them, just like before!  We'll tell you what you forgot to check for
instead of just segfaulting, though.

Features
--------

* Automatically initializes as functions are used.
* Can be mixed with non-Epoxy GL usage.
* Knows about function aliases.
* OpenGL Core Context Support
* OpenGL ES Context Support
* EGL, GLX, & WGL Support

Building Epoxy
--------------

You will need the following tools at a minimum in addition to the standard
compilers for your system (GCC, Xcode/Clang, MinGW, MSVC, etc.).

* Python 2.7+
* CMake 2.8+

Just compile as usual for a CMake project:

	mkdir build
	cd build
	cmake ..
	make && make test

and optionally:
	
	make install

Using Epoxy
-----------

The only headers you need to include are:

	#include <epoxy/gl.h>  // instead of <GL/gl.h> and <GL/glext.h>
	#include <epoxy/glx.h> // instead of <GL/glx.h> and <GL/glxext.h> (for GLX)
	#include <epoxy/egl.h> // instead of <EGL/egl.h> and <EGL/eglext.h> (for EGL)
	#include <epoxy/wgl.h> // instead of <windows.h> and <GL/wglext.h> (for WGL)

The other headers aren't needed, but can be included as long as Epoxy's headers
are included first.

Epoxy also defines some helper functions:

* ```int epoxy_gl_version()```: Returns GL Version
  * ```12``` for OpenGL 1.2
  * ```20``` for OpenGL 2.0
  * ```44``` for OpenGL 4.4
* ```bool epoxy_has_gl_extension()```: Returns Whether GL Extension Available
  * Keep this out of hot paths as it is not terribly performant.

Windows Usage Notes
-------------------

**IMPORTANT**

If you are using MSVC and have a **DLL** build of Epoxy, you will need to
```#define EPOXY_DLL``` prior to including the headers so the functions are
decorated with ```__declspec(dllimport)``` as needed. This is not required if
building with a ~~better~~different compiler such as MinGW GCC.

The automatic per-context symbol resolution for the Windows platform requires
that Epoxy knows when ```wglMakeCurrent()``` is called. This is because
``wglGetProcAddress()`` return values depend on the context's device and pixel
format.  If ```wglMakeCurrent()``` is called from outside of Epoxy in a way
that might change the device or pixel format, then Epoxy needs to be notified
of the change using the ```epoxy_handle_external_wglMakeCurrent()``` function.

The ```wglMakeCurrent()``` variants are slower than they should be because they
should be caching the resolved dispatch tables instead of resetting an entire
thread-local dispatch table every time. This is an area for future improvement.
