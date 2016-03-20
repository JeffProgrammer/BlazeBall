The BlazeBall Engine
=============

A game engine that was made for a 3D marble project. It has since been released under the BSD license because it became too much of a hastle to make a commercial game AND the engine, instead of focusing on the game itself. Hopefully people will find some use for this code as long as they abide by the License. See the LICENSE file for more information.

Building
-------------------

Run CMake on the root project directory. It is recommended to build project/binary
files to another directory, such as `build/`.

Running
-------------------

Make the "install" target from the CMakeFile and run the `Game` executable in `bin/`.  
The executable is run as `Game <dif file> [dif file 2, ...]`. Files can be in any compatible DIF format.

Requirements
-------------------

C++14 supported compiler. MSVC 2015, Clang 3.5+ 
OpenGL 3.3, although OpenGL 2.1 should work if mainstream 3.x extensions are supported.

* Windows 7+
* Mac OSX 10.10.5+

Tested on Nvidia and Intel cards using their latest drivers. ATI will most likely work.

Although not tested, this code should run on older operating systems such as Windows XP and Mac OSX Snow Leopard or higher. It was never tested on Linux, but considering that it works on MacOSX and we use third party libraries with UNIX compatability, it shouldn't be to bad to get it up and running.

Third Party Libraries
-------------------

* Assimp - Model Importer
* Bullet3D - Physics Library
* ENet - Networking Library
* ENetPP - Networking extensions library for ENet
* FreeType - Font Library
* Epoxy - OpenGL Extensions Loader
* GLM - Math Library
* JPEG - Image Library
* LibRocket - GUI library
* MNG - Image Library
* RapidJSON - JSON parser library
* SDL2 - Window and Input System
* srutil - Fast C++ Delegates
* ZLIB - Compression Library

License
-------------------

```
Copyright (c) 2014-2016, Glenn Smith
Copyright (c) 2014-2016, Jeff Hutchinson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
